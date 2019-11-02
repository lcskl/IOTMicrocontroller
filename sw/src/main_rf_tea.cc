m/*
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

#include "RF24.h"
#include "tea.h"

#define CE_PIN 2
#define CSN_PIN 5
#define MOSI_PIN 4
#define MISO_PIN 0
#define SCK_PIN 1

//XGpio GpioOutput;

int main ()
{
   Xil_ICacheEnable();
   Xil_DCacheEnable();
   print("---Entering main---\n\r");
   print("\r\n");

   GPIO::init();

   uint32_t result[2];
   //uint32_t result_dec[2];
   uint32_t word[2] = {0xBEBACAFE, 0xDEADBEEF};
   uint32_t keys[4] = {0x01, 0x02, 0x03, 0x04};

   init(teaREG1, keys);
   encrypt(teaREG1, word, result);
   //decrypt(teaREG1, result, result_dec);

   const uint8_t address[6] = "00001";

   RF24 radio(CE_PIN, CSN_PIN);

   radio.begin();
   radio.openWritingPipe(address);
   radio.setPALevel(RF24_PA_MIN);
   radio.stopListening();


   while(1) {
	   //const char text[] = "Hello World";

	   radio.write(result, 8);
	   //delay(1000);
	   for(int i = 0; i < 10000000; i++) {}
   }

   print("---Exiting main---\n\r");
   Xil_DCacheDisable();
   Xil_ICacheDisable();
   return 0;
}
