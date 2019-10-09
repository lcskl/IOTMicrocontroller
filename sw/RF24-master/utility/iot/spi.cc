#include "spi.h"

SPI::SPI() {
	GPIO::open(this->clkPin, GPIO::DIRECTION_OUT);
	GPIO::open(this->dataPin, GPIO::DIRECTION_OUT);
	GPIO::open(this->misoPin, GPIO::DIRECTION_IN);

	GPIO::write(this->clkPin, GPIO::OUTPUT_LOW);
}

void SPI::begin() {

}

uint8_t SPI::transfer(uint8_t tx_) {
	uint8_t status;
	status = this->shiftInOut(tx_);

	return status;
}

void SPI::transfernb(char* tbuf, char* rbuf, uint32_t len) {

}

void SPI::transfern(char* buf, uint32_t len) {
	uint8_t i;

	for(i = 0; i < len; i++) {
		this->transfer(buf[i]);
	}
}

SPI::~SPI() {

}


uint8_t SPI::shiftInOut(uint8_t data) {
	uint8_t i;
	uint8_t data_in = 0;
	for(i = 0; i < 8; i++){
		uint8_t val = !!(data & (1 << (7 - i)));
		GPIO::write(this->dataPin, val);
		data_in = (data_in << 1) | GPIO::read(this->misoPin);
	    GPIO::write(this->clkPin , GPIO::OUTPUT_HIGH);
	    GPIO::write(this->clkPin , GPIO::OUTPUT_LOW);
	}
	return data_in;
}
