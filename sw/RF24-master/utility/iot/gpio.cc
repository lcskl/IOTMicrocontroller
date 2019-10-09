#include "gpio.h"

static XGpio mygpio;
static int is_init = 0
		;
GPIO::GPIO() {

}

GPIO::~GPIO() {

}

void GPIO::init() {
	uint32_t status;
	
	if(is_init == 0) {
		is_init = 1;
		status = XGpio_Initialize(&mygpio, XPAR_AXI_GPIO_0_DEVICE_ID);
		if (status != XST_SUCCESS) {
			//printf("ERROR: GPIO init failed");
		}
	}
}

void GPIO::open(int port, int DDR) {

 //  XGpio_SetDataDirection(&this->mygpio, 1, 0x0); // All output

	uint32_t current_port;

	if(GPIO::assert_port(port)) {
		port = 1 << port; // Change binary to bitmask
		current_port = XGpio_GetDataDirection(&mygpio, 1);
		
		if(DDR == GPIO::DIRECTION_OUT) {
			XGpio_SetDataDirection(&mygpio, 1, current_port & ~port);
		}
		else {
			XGpio_SetDataDirection(&mygpio, 1, current_port | port);
		}
	}
	

}

// Unnecessary
void GPIO::close(int port) {

}

int GPIO::read(int port) {
	uint32_t current_port;

	GPIO::assert_port(port);
	current_port = XGpio_DiscreteRead(&mygpio, 1);

	return ((1 << port) & current_port) ? 1 : 0; // Return port-specified value.
}

void GPIO::write(int port, int value) {

	GPIO::assert_port(port);
	port = 1 << port;
	if(value == GPIO::OUTPUT_LOW) {
		XGpio_DiscreteClear(&mygpio, 1, port);
	}
	else {
		XGpio_DiscreteSet(&mygpio, 1, port);
	}

}

int GPIO::assert_port(int port) {
	if(port >= 0 && port < GPIO::PORT_SIZE) {
		return 1;
	}
	else {
		//printf("ERROR: GPIO port greater than permitted");
		return 0;
	}
}
