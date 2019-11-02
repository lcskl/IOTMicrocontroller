/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xparameters.h"
#include "xiic.h"
#include "xiic_l.h"

#include "MAX30100.h"

XIic Iic; /* The driver instance for IIC Device */

MAX30100::MAX30100()
{
}

bool MAX30100::begin()
{

	//Wire.begin();
    //Wire.setClock(I2C_BUS_SPEED);
	// Xilinx I2C init
	int Status;
	XIic_Config *ConfigPtr;	/* Pointer to configuration data */

	//We need to initialize the timer
	//initTime(XPAR_AXI_TIMER_0_DEVICE_ID, 0x00);

	/*
	 * Initialize the IIC driver so that it is ready to use.
	 */
	ConfigPtr = XIic_LookupConfig(XPAR_AXI_IIC_0_DEVICE_ID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	Status = XIic_CfgInitialize(&Iic, ConfigPtr, ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XIic_SetAddress(&Iic, XII_ADDR_TO_SEND_TYPE,
			MAX30100_I2C_ADDRESS);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	// Library
    if (getPartId() != EXPECTED_PART_ID) {
        return false;
    }

    setMode(DEFAULT_MODE);
    setLedsPulseWidth(DEFAULT_PULSE_WIDTH);
    setSamplingRate(DEFAULT_SAMPLING_RATE);
    setLedsCurrent(DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);
    setHighresModeEnabled(true);

    return true;
}

void MAX30100::setMode(Mode mode)
{
    writeRegister(MAX30100_REG_MODE_CONFIGURATION, mode);
}

void MAX30100::setLedsPulseWidth(LEDPulseWidth ledPulseWidth)
{
    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);
}

void MAX30100::setSamplingRate(SamplingRate samplingRate)
{
    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));
}

void MAX30100::setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent)
{
    writeRegister(MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);
}

void MAX30100::setHighresModeEnabled(bool enabled)
{
    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    if (enabled) {
        writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
    } else {
        writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
    }
}

void MAX30100::update()
{
    readFifoData();
}

bool MAX30100::getRawValues(uint16_t *ir, uint16_t *red)
{
    if (!readoutsBuffer.isEmpty()) {
        SensorReadout readout = readoutsBuffer.pop();

        *ir = readout.ir;
        *red = readout.red;

        return true;
    } else {
        return false;
    }
}

void MAX30100::resetFifo()
{
    writeRegister(MAX30100_REG_FIFO_WRITE_POINTER, 0);
    writeRegister(MAX30100_REG_FIFO_READ_POINTER, 0);
    writeRegister(MAX30100_REG_FIFO_OVERFLOW_COUNTER, 0);
}


/*****************************************************************************/
/**
* This function writes a buffer of bytes and reads the returned bytes on IIC bus.
*
* @param	AXI_IIC_BASE_ADDRESS contains the base address of the AXI IIC Block. This is not the same as the device ID.
* @param	i2c_device_address contains the address of the device we will be transmitting to
* @param    internal_register_address is the address of the register inside of IC we would like to read from
* @param	rx_buf contains the number of bytes in the buffer to be received
* @param	rx_size contains the number of bytes in rx_buf
*
* @return
* 		-XST_SUCCESS if Successful else XST_FAILURE.
*
*
******************************************************************************/
int PL_AXI_I2C_TX_RX(int AX_IIC_BASE_ADDRESS, u8 i2c_device_address, u8 internal_register_address, u8 *rx_buf, u16 rx_size)
{
	volatile unsigned ReceivedByteCount;
	u16 StatusReg;

	/*
	 * Set the address register to the specified address by writing
	 * the address to the device, this must be tried until it succeeds
	 * because a previous write to the device could be pending and it
	 * will not ack until that write is complete.
	 *
	 * A repeated stat condition is manditory for certain ICs
	 */
	do {
		StatusReg = XIic_ReadReg(AX_IIC_BASE_ADDRESS, XIIC_SR_REG_OFFSET);
		if(!(StatusReg & XIIC_SR_BUS_BUSY_MASK)) {
			ReceivedByteCount = XIic_Send(AX_IIC_BASE_ADDRESS,
							i2c_device_address,
							(u8 *)&internal_register_address,
							sizeof(internal_register_address),
							XIIC_REPEATED_START);

			if (ReceivedByteCount != sizeof(internal_register_address)) {

				/* Send is aborted so reset Tx FIFO */
				XIic_WriteReg(AX_IIC_BASE_ADDRESS,
						XIIC_CR_REG_OFFSET,
						XIIC_CR_TX_FIFO_RESET_MASK);
				XIic_WriteReg(AX_IIC_BASE_ADDRESS,
						XIIC_CR_REG_OFFSET,
						XIIC_CR_ENABLE_DEVICE_MASK);
			}
		}

	} while (ReceivedByteCount != sizeof(internal_register_address));

	/*
	 * Read the number of bytes at the specified internal address.
	 */
	ReceivedByteCount = XIic_Recv(AX_IIC_BASE_ADDRESS, i2c_device_address,
					rx_buf, rx_size, XIIC_STOP);

	/*
	 * See if we got all byes back that we expected.
	 */
	if(ReceivedByteCount != rx_size){
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

uint8_t MAX30100::readRegister(uint8_t address)
{
	uint8_t read;

    //Wire.beginTransmission(MAX30100_I2C_ADDRESS);
    //Wire.write(address);
    //Wire.endTransmission(false);
    //Wire.requestFrom(MAX30100_I2C_ADDRESS, 1);
	//read = Wire.read();
	// Mudar

	/*
	XIic_Send(XPAR_AXI_IIC_0_DEVICE_ID, MAX30100_I2C_ADDRESS,
				   &address, 1, XIIC_REPEATED_START);

	XIic_Recv(XPAR_AXI_IIC_0_DEVICE_ID, MAX30100_I2C_ADDRESS,
				&read, 1, XIIC_STOP);
	*/
	//uint8_t msg[2];
	//msg[0] =
	//XIIC_REPEATED_START
	XIic_Start(&Iic);
	//XIic_Send(XPAR_AXI_IIC_0_BASEADDR, MAX30100_I2C_ADDRESS, &address, 1, XIIC_STOP);
	//XIic_Recv(XPAR_AXI_IIC_0_BASEADDR, MAX30100_I2C_ADDRESS, &read, 1, XIIC_STOP);

	//XIic_Stop(&Iic);

	PL_AXI_I2C_TX_RX(XPAR_AXI_IIC_0_BASEADDR, MAX30100_I2C_ADDRESS, address, &read, 1);
	return read;

}

/*****************************************************************************/
/**
* This function writes a buffer of bytes to the IIC bus.
*
* @param	AX_IIC_BASE_ADDRESS contains the base address of the AXI IIC Block. This is not the same as the device ID.
* @param	i2c_device_address contains the address of the device we will be transmitting to
* @param    internal_register_address is the address of the register inside of IC we would like to write to
* @param	tx_buf contains the number of bytes in the buffer to be transmitted (typically a register address we want to read)
* @param	tx_size contains the number of bytes in tx_buf
*
* @return
* 		-XST_SUCCESS if Successful else XST_FAILURE.
*
*
******************************************************************************/
int PL_AXI_I2C_TX(int AX_IIC_BASE_ADDRESS, u8 i2c_device_address, u8 internal_register_address, u8 *tx_buf, u16 tx_size)
{
	volatile unsigned SentByteCount;
	volatile unsigned AckByteCount;
	u8 WriteBuffer[sizeof(internal_register_address) + tx_size];
	int Index;

	/*
	 * A temporary write buffer must be used which contains both the address
	 * and the data to be written, put the address in first based upon the
	 * size of the address for the EEPROM.
	 */
	WriteBuffer[0] = (u8)(internal_register_address);

	/*
	 * Put the data in the write buffer following the address.
	 */
	for (Index = 0; Index < tx_size; Index++) {
		WriteBuffer[sizeof(internal_register_address) + Index] = tx_buf[Index];
	}

	/*
	 * Set the address register to the specified address by writing
	 * the address to the device, this must be tried until it succeeds
	 * because a previous write to the device could be pending and it
	 * will not ack until that write is complete.
	 */
	do {
		SentByteCount = XIic_Send(AX_IIC_BASE_ADDRESS,
					i2c_device_address,
					(u8 *)&internal_register_address, sizeof(internal_register_address),
					XIIC_STOP);
		if (SentByteCount != sizeof(internal_register_address)) {

			/* Send is aborted so reset Tx FIFO */
			XIic_WriteReg(AX_IIC_BASE_ADDRESS,  XIIC_CR_REG_OFFSET,
					XIIC_CR_TX_FIFO_RESET_MASK);
			XIic_WriteReg(AX_IIC_BASE_ADDRESS, XIIC_CR_REG_OFFSET,
					XIIC_CR_ENABLE_DEVICE_MASK);
		}

	} while (SentByteCount != sizeof(internal_register_address));
	/*
	 * Write a page of data at the specified address to the EEPROM.
	 */
	SentByteCount = XIic_Send(AX_IIC_BASE_ADDRESS, i2c_device_address,
				  WriteBuffer, sizeof(internal_register_address) + tx_size,
				  XIIC_STOP);

	/*
	 * Wait for the write to be complete by trying to do a write and
	 * the device will not ack if the write is still active.
	 */
	do {
		AckByteCount = XIic_Send(AX_IIC_BASE_ADDRESS, i2c_device_address,
					(u8 *)&internal_register_address, sizeof(internal_register_address),
					XIIC_STOP);
		if (AckByteCount != sizeof(internal_register_address)) {

			/* Send is aborted so reset Tx FIFO */
			XIic_WriteReg(AX_IIC_BASE_ADDRESS,  XIIC_CR_REG_OFFSET,
					XIIC_CR_TX_FIFO_RESET_MASK);
			XIic_WriteReg(AX_IIC_BASE_ADDRESS, XIIC_CR_REG_OFFSET,
					XIIC_CR_ENABLE_DEVICE_MASK);
		}

	} while (AckByteCount != sizeof(internal_register_address));

	/*
	 * Return success if number of bytes written includes size of i2c_device_address
	 * and tx_size
	 */
	if(SentByteCount != (sizeof(internal_register_address)+tx_size)){
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}


void MAX30100::writeRegister(uint8_t address, uint8_t data)
{
    //Wire.beginTransmission(MAX30100_I2C_ADDRESS);
    //Wire.write(address);
    //Wire.write(data);
    //Wire.endTransmission();
	/*
	XIic_Send(XPAR_AXI_IIC_0_DEVICE_ID, MAX30100_I2C_ADDRESS,
				   &address, 1, XIIC_REPEATED_START);

	XIic_Send(XPAR_AXI_IIC_0_DEVICE_ID, MAX30100_I2C_ADDRESS,
			   &data, 1, XIIC_STOP);
	*/
	PL_AXI_I2C_TX(XPAR_AXI_IIC_0_BASEADDR, MAX30100_I2C_ADDRESS, address, &data, 1);
}

void MAX30100::burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length)
{
    //Wire.beginTransmission(MAX30100_I2C_ADDRESS);
    //Wire.write(baseAddress);
    //Wire.endTransmission(false);
    //Wire.requestFrom((uint8_t)MAX30100_I2C_ADDRESS, length);

    //uint8_t idx = 0;
    //while (Wire.available()) {
    //    buffer[idx++] = Wire.read();
    //}
	XIic_Send(XPAR_AXI_IIC_0_DEVICE_ID, MAX30100_I2C_ADDRESS,
				   &baseAddress, 1, XIIC_REPEATED_START);

	XIic_Recv(XPAR_AXI_IIC_0_DEVICE_ID, MAX30100_I2C_ADDRESS,
				buffer, length, XIIC_STOP);

}

void MAX30100::readFifoData()
{
    uint8_t buffer[MAX30100_FIFO_DEPTH*4];
    uint8_t toRead;

    toRead = (readRegister(MAX30100_REG_FIFO_WRITE_POINTER) - readRegister(MAX30100_REG_FIFO_READ_POINTER)) & (MAX30100_FIFO_DEPTH-1);

    if (toRead) {
        burstRead(MAX30100_REG_FIFO_DATA, buffer, 4 * toRead);

        for (uint8_t i=0 ; i < toRead ; ++i) {
            // Warning: the values are always left-aligned
            readoutsBuffer.push({
                    .ir=(uint16_t)((buffer[i*4] << 8) | buffer[i*4 + 1]),
                    .red=(uint16_t)((buffer[i*4 + 2] << 8) | buffer[i*4 + 3])});
        }
    }
}

void MAX30100::startTemperatureSampling()
{
    uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
    modeConfig |= MAX30100_MC_TEMP_EN;

    writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

bool MAX30100::isTemperatureReady()
{
    return !(readRegister(MAX30100_REG_MODE_CONFIGURATION) & MAX30100_MC_TEMP_EN);
}

float MAX30100::retrieveTemperature()
{
    int8_t tempInteger = readRegister(MAX30100_REG_TEMPERATURE_DATA_INT);
    float tempFrac = readRegister(MAX30100_REG_TEMPERATURE_DATA_FRAC);

    return tempFrac * 0.0625 + tempInteger;
}

void MAX30100::shutdown()
{
    uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
    modeConfig |= MAX30100_MC_SHDN;

    writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

void MAX30100::resume()
{
    uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
    modeConfig &= ~MAX30100_MC_SHDN;

    writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

uint8_t MAX30100::getPartId()
{
    return readRegister(0xff);
}
