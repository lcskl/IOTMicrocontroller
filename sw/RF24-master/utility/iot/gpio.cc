#include "gpio.h"

GPIO::GPIO() {
	uint32_t status;
	
	status = XGpio_Initialize(&this->mygpio, XPAR_AXI_GPIO_0_DEVICE_ID);
	if (status != XST_SUCCESS) {
		printf("ERROR: GPIO init failed");
	}
}

GPIO::~GPIO() {

}

void GPIO::open(int port, int DDR) {

 //  XGpio_SetDataDirection(&this->mygpio, 1, 0x0); // All output

	uint32_t current_port;

	if(this->assert_port(port)) {
		port = 1 << port; // Change binary to bitmask
		current_port = XGpio_GetDataDirection(&this->mygpio, 1);
		
		if(DDR == this->DIRECTION_OUT) {
			XGpio_SetDataDirection(&this->mygpio, 1, current_port & ~port);
		}
		else {
			XGpio_SetDataDirection(&this->mygpio, 1, current_port | port);
		}
	}
	

}

// Unnecessary
void GPIO::close(int port) {

}

int GPIO::read(int port) {
	uint32_t current_port;

	this->assert_port(port);
	current_port = XGpio_DiscreteRead(&this->mygpio, 1);

	return ((1 << port) & current_port) ? 1 : 0; // Return port-specified value.
}

void GPIO::write(int port, int value) {

	this->assert_port(port);
	port = 1 << port;
	if(value == this->OUTPUT_LOW) {
		XGpio_DiscreteClear(&this->mygpio, 1, port);
	}
	else {
		XGpio_DiscreteSet(&this->mygpio, 1, port);
	}

}

int GPIO::assert_port(int port) {
	if(port >= 0 && port < this->PORT_SIZE) {
		return 1;
	}
	else {
		printf("ERROR: GPIO port greater than permitted");
		return 0;
	}
}
