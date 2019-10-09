

/**
 * @file gpio.h
 * Class declaration for SPI helper files
 */

 /**
 * Example of gpio.h class declaration for GPIO portability
 *
 * @defgroup Porting_GPIO Porting: GPIO
 *
 * 
 * @{
 */
#ifndef IOT_GPIO_H
#define	IOT_GPIO_H

#include <stdint.h>
#include <stdio.h>
#include "xgpio.h"
#include "xparameters.h"

class GPIO {

XGpio mygpio;

public:
 	/* Constants */
	static const int DIRECTION_OUT = 0;
	static const int DIRECTION_IN = 1;
	
	static const int OUTPUT_HIGH = 1;
	static const int OUTPUT_LOW = 0;
	
	static const int PORT_SIZE = 8;

	GPIO();
	
	/**
	 * Similar to Arduino pinMode(pin,mode);
     * @param port
     * @param DDR
     */
	void open(int port, int DDR);
	/**
	 * 
     * @param port
     */
	void close(int port);
	/**
	 * Similar to Arduino digitalRead(pin);
     * @param port
     * @param value
     */
	int read(int port);
	/**
	* Similar to Arduino digitalWrite(pin,state);
	* @param port
	* @param value
	*/	
	void write(int port,int value);	
	
	virtual ~GPIO();

private:
	int assert_port(int port);
};

#endif
/*@}*/	
