
/**
 * @file spi.h
 * Class declaration for SPI helper files
 */

 /**
 * Example of spi.h class declaration for SPI portability
 *
 * @defgroup Porting_SPI Porting: SPI
 *
 * 
 * @{
 */
#ifndef IOT_SPI_H
#define IOT_SPI_H

#include <stdint.h>
#include <stdio.h>

#include "gpio.h"


#define CE_PIN 2
#define CSN_PIN 5
#define MOSI_PIN 4
#define MISO_PIN 0
#define SCK_PIN 1

using namespace std;
class SPI {
public:

/**
	* SPI constructor
	*/	 
	SPI();
	
	/**
	* Start SPI
	*/
	void begin();
	
	/**
	* Transfer a single byte
	* @param tx_ Byte to send
	* @return Data returned via spi
	*/
	uint8_t transfer(uint8_t tx_);
	
	/**
	* Transfer a buffer of data
	* @param tbuf Transmit buffer
	* @param rbuf Receive buffer
	* @param len Length of the data
	*/
	void transfernb(char* tbuf, char* rbuf, uint32_t len);

	/**
	* Transfer a buffer of data without an rx buffer
	* @param buf Pointer to a buffer of data
	* @param len Length of the data
	*/	
	void transfern(char* buf, uint32_t len);
	
	virtual ~SPI();

private:

	static int const clkPin = SCK_PIN;
	static int const dataPin = MOSI_PIN;
	static int const misoPin = MISO_PIN;

	void init();	
	uint8_t shiftInOut(uint8_t data);
};


/*@}*/
#endif
