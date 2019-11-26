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
#include <float.h>

#include "xparameters.h"
#include "xil_cache.h"
#include "xgpio.h"
#include "gpio.h"

#include "MAX30100.h"
#include "RF24.h"
#include "tea.h"

#define CE_PIN 2
#define CSN_PIN 5
#define MOSI_PIN 4
#define MISO_PIN 0
#define SCK_PIN 1

MAX30100 sensor;

float my_random_temp() {
	float temperature;

	sensor.startTemperatureSampling();
	temperature = sensor.retrieveTemperature();
	//float unit = (float)getCountTime() / (float)0xFFFFFFFF;
	return temperature + 6.5;
}

float my_random_bpm() {

	float temperature;

	sensor.startTemperatureSampling();
	temperature = sensor.retrieveTemperature()*2.5;
	return temperature + 6.5;

}

float my_random_oximetry() {

	float temperature;

	sensor.startTemperatureSampling();
	temperature = sensor.retrieveTemperature()/30;
	return temperature * 95;

}

int main ()
{
   Xil_ICacheEnable();
   Xil_DCacheEnable();
   print("---Entering main---\n\r");
   print("\r\n");

   GPIO::init();

   uint32_t result[4];
   uint32_t word[4] = {0xBEBACAFE, 0xDEADBEEF, 0xBEBACAFE, 0xDEADBEEF};
   uint32_t keys[4] = {0x01, 0x02, 0x03, 0x04};

   init(teaREG1, keys);

   const uint8_t address[6] = "00001";


   if (!sensor.begin()) {
       print("FAILED: \n\r");

       uint8_t partId = sensor.getPartId();
       if (partId == 0xff) {
           print("I2C error\n\r");
       } else {
           print("wrong part ID 0x");
           xil_printf("%x ", partId);
           print(" (expected: 0x");
           xil_printf("%x\n\r", EXPECTED_PART_ID);
       }
       // Stop here
       for(;;);
   } else {
       print("Success\n\r");
   }


   RF24 radio(CE_PIN, CSN_PIN);

   radio.begin();
   radio.openWritingPipe(address);
   radio.setPALevel(RF24_PA_MIN);
   radio.stopListening();


   while(1) {
	   sensor.startTemperatureSampling();
	   temperature = 1000 * sensor.retrieveTemperature();

	   uint32_t temp = 1000*my_random_temp();
	   uint32_t bpm = 1000*my_random_bpm();
	   uint32_t oxi = 1000*my_random_oximetry();

	   print("Sending ... \n\r");
	   word[0] = temp ;
	   word[1] = temp ;
	   word[2] = bpm ;
	   word[3] = oxi ;
	   encrypt(teaREG1, word, result);
	   encrypt(teaREG1, word+2, result+2);
	   radio.write(result, 8);
	   radio.write(result+2, 8);

	   for(int i = 0; i < 1000000; i++) {}
   }

   print("---Exiting main---\n\r");
   Xil_DCacheDisable();
   Xil_ICacheDisable();
   return 0;
}
