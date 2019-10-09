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

#include "RF24.h"

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

   //GPIO::open(4, GPIO::DIRECTION_OUT);
   //GPIO::write(4, GPIO::OUTPUT_HIGH);
/*
   u32 status;
   int led_bit = 1;
   status = XGpio_Initialize(&GpioOutput, XPAR_AXI_GPIO_0_DEVICE_ID);
   XGpio_SetDataDirection(&GpioOutput, LED_CHANNEL, 0x0); // All output
   if (status == XST_SUCCESS) {
	   while (1) {
		   XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, led_bit);
		   if (led_bit){
			   print("Turn on...\n\r");
		   }
		   else {
			   print("Turn off...\n\r");
		   }
		   for (int i = 0; i < 10000000; i++) {};
		   led_bit = led_bit << 1;
   	   }
   }
*/

/*
	// Write test
   GPIO gp;
   gp.open(0, GPIO::DIRECTION_OUT);
   gp.open(1, GPIO::DIRECTION_OUT);
   gp.open(2, GPIO::DIRECTION_OUT);
   gp.open(3, GPIO::DIRECTION_OUT);

   int val = GPIO::OUTPUT_LOW;
   gp.write(1, GPIO::OUTPUT_LOW);
   gp.write(2, GPIO::OUTPUT_LOW);
   gp.write(3, GPIO::OUTPUT_LOW);
   while (1) {
	   for (int i = 0; i < 10000000; i++) {

   	   }
	   printf("Toggle\n\r");
	   val = ~val;
   	   gp.write(0, val);
   	   gp.write(1, val);
   	   gp.write(2, val);
   	   gp.write(3, val);

   }
*/

/*
   // Read test
   int read;
   GPIO gp;

   gp.open(4, GPIO::DIRECTION_IN);

   while (1) {
	   read = gp.read(4);
	   printf("Read %d\n\r", read);
	   for(int i = 0; i < 10000000; i++) {}
   }
*/
   const uint8_t address[6] = "00001";

   RF24 radio(CE_PIN, CSN_PIN);

   radio.begin();
   radio.openWritingPipe(address);
   radio.setPALevel(RF24_PA_MIN);
   radio.stopListening();


   while(1) {
	   const char text[] = "Hello World";
	   radio.write(&text, sizeof(text));
	   //delay(1000);
	   for(int i = 0; i < 10000000; i++) {}
   }

   print("---Exiting main---\n\r");
   Xil_DCacheDisable();
   Xil_ICacheDisable();
   return 0;
}
