/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 *
 *
 * This file is a generated sample test application.
 *
 * This application is intended to test and/or illustrate some
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * SDK application project when you run the "Generate Libraries" menu item.
 *
 */

//#include <stdio.h>
#include "xparameters.h"
#include "xil_cache.h"
#include "xgpio.h"
#include "gpio.h"

#include "MAX30100.h"
#include "RF24.h"
#include "tea.h"

// Sampling is tightly related to the dynamic range of the ADC.
// refer to the datasheet for further info
//#define SAMPLING_RATE                       MAX30100_SAMPRATE_100HZ
#define SAMPLING_RATE						MAX30100_SAMPRATE_1000HZ

// The LEDs currents must be set to a level that avoids clipping and maximises the
// dynamic range
//#define IR_LED_CURRENT                      MAX30100_LED_CURR_50MA
//#define RED_LED_CURRENT                     MAX30100_LED_CURR_27_1MA

#define IR_LED_CURRENT                      MAX30100_LED_CURR_50MA
#define RED_LED_CURRENT                     MAX30100_LED_CURR_50MA

// The pulse width of the LEDs driving determines the resolution of
// the ADC (which is a Sigma-Delta).
// set HIGHRES_MODE to true only when setting PULSE_WIDTH to MAX30100_SPC_PW_1600US_16BITS
#define PULSE_WIDTH                         MAX30100_SPC_PW_1600US_16BITS
#define HIGHRES_MODE                        true

// Instantiate a MAX30100 sensor class
MAX30100 sensor;

int main ()
{
   Xil_ICacheEnable();
   Xil_DCacheEnable();
   print("---Entering main---\n\r");
   print("\r\n");

   GPIO::init();

   print("Initializing MAX30100..\n\r");

   // Initialize the sensor
   // Failures are generally due to an improper I2C wiring, missing power supply
   // or wrong target chip
   if (!sensor.begin()) {
       print("FAILED\n\r");
       for(;;);
   } else {
       print("SUCCESS\n\r");
   }

   // Set up the wanted parameters
   sensor.setMode(MAX30100_MODE_SPO2_HR);
   sensor.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT);
   sensor.setLedsPulseWidth(PULSE_WIDTH);
   sensor.setSamplingRate(SAMPLING_RATE);
   sensor.setHighresModeEnabled(HIGHRES_MODE);


   volatile uint8_t sesor_mode;
   volatile uint8_t red_ir_config;
   volatile uint8_t reg_spo_config;
   volatile uint8_t wp;

   red_ir_config = sensor.readRegister(MAX30100_REG_LED_CONFIGURATION);
   sesor_mode = sensor.readRegister(MAX30100_REG_MODE_CONFIGURATION);
   reg_spo_config = sensor.readRegister(MAX30100_REG_SPO2_CONFIGURATION);



   while(1){
	    uint16_t ir, red;

	    wp = sensor.readRegister(MAX30100_REG_FIFO_WRITE_POINTER);

	    sensor.update();



	    while (sensor.getRawValues(&ir, &red)) {
	        xil_printf("%d ", ir);
	        xil_printf("%d \n\r", red);
	    }
   }

   print("---Exiting main---\n\r");
   Xil_DCacheDisable();
   Xil_ICacheDisable();
   return 0;
}
