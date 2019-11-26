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

#define CE_PIN 2
#define CSN_PIN 5
#define MOSI_PIN 4
#define MISO_PIN 0
#define SCK_PIN 1

MAX30100 sensor;

int main ()
{
   Xil_ICacheEnable();
   Xil_DCacheEnable();
   print("---Entering main---\n\r");
   print("\r\n");

   GPIO::init();

   uint8_t cnt = 0;
   uint32_t result[2];
   uint32_t result_dec[2];
   uint32_t word[2] = {0xBEBACAFE, 0xDEADBEEF};
   uint32_t keys[4] = {0x01, 0x02, 0x03, 0x04};
   uint32_t temperature;

   init(teaREG1, keys);
   //encrypt(teaREG1, word, result);
   //decrypt(teaREG1, result, result_dec);

   const uint8_t address[6] = "00001";


   if (!sensor.begin()) {
       print("FAILED: ");

       uint8_t partId = sensor.getPartId();
       if (partId == 0xff) {
           print("I2C error\n");
       } else {
           print("wrong part ID 0x");
           xil_printf("%x ", partId);
           print(" (expected: 0x");
           xil_printf("%x\n\r", EXPECTED_PART_ID);
       }
       // Stop here
       for(;;);
   } else {
       print("Success\n");
   }


   RF24 radio(CE_PIN, CSN_PIN);

   radio.begin();
   radio.openWritingPipe(address);
   radio.setPALevel(RF24_PA_MIN);
   radio.stopListening();

   /*
   sensor.startTemperatureSampling();
   temperature = 1000 * sensor.retrieveTemperature();
   word[0] = 0x11117acd;

   sensor.startTemperatureSampling();
   temperature = 1000 * sensor.retrieveTemperature();
   word[1] = 0x11117ac8f;
   encrypt(teaREG1, word, result);
   decrypt(teaREG1, result, result_dec);
   */
   while(1) {
	   sensor.startTemperatureSampling();
	   temperature = 1000 * sensor.retrieveTemperature();

	   //radio.write(result, 8);
	   word[cnt] = temperature ;//| 0xFFFF0000;

	   cnt++;
	   if(cnt >= 2){
	   		encrypt(teaREG1, &word[0], &result[0]);
	   		radio.write(result, 8);
	   		decrypt(teaREG1, &result[0], &result_dec[0]);
	   		cnt = 0;
	   }

	   for(int i = 0; i < 10000000; i++) {}
   }

   print("---Exiting main---\n\r");
   Xil_DCacheDisable();
   Xil_ICacheDisable();
   return 0;
}
