#include "spi.h"

SPI::SPI() {
	this->gp.open(GPIO::DIRECTION_OUT, this->clkPin);
	this->gp.open(GPIO::DIRECTION_OUT, this->dataPin);

	this->gp.write(this->clkPin, GPIO::OUTPUT_LOW);
}

void SPI::begin() {

}

uint8_t SPI::transfer(uint8_t tx_) {
	this->shiftOut(tx_);

	return 0;
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


void SPI::shiftOut(uint8_t data) {
	uint8_t i;

	for(i = 0; i < 8; i++){
		gp.write(this->dataPin, !!(data & (1 << (7 - i))));
	    gp.write(this->clkPin , GPIO::OUTPUT_HIGH);
	    gp.write(this->clkPin , GPIO::OUTPUT_LOW);
	}

}
