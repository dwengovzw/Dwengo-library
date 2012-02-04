#include "dwengoSensorModule.h"
#include "dwengoDelay.h"
#include "dwengoI2C.h"


#define MAX11604_ADDRESS 0b11001010
#define MAX7320_ADDRESS  0b10110000

// todo: TSOP -> integreer met dwengoRC library -> pas dwengoRC aan zodat met sensormodule werkt, dit houdt een verandering van RB4 naar RB2 in en activeren van de TSOP

unsigned char readMAX7320(void);
void writeMAX7320(unsigned char);
unsigned char readMAX11604(void);
void writeMAX11604(unsigned char channel);

void initSensorModule() {
	unsigned char dummy;
	unsigned int block_cnt;
	
	initI2C();
	write1I2C(MAX11604_ADDRESS, 0b10000010);	// setup MAX11604
}

void powerLongRange(unsigned char state) {
	unsigned char status;
	status = readMAX7320();
	if(state == TRUE)
		writeMAX7320(status&0b11110111); // long range active (dgrp3 must be 0)
	else
		writeMAX7320(status|0b00001000); // long range unactive
}

void setHeadlights(unsigned char side, unsigned char state) {
	unsigned char status;
	status = readMAX7320();
	delay_us(50);
	if(side == LD1) {
		if(state == TRUE)
			writeMAX7320(status|0b00100000);
		else
			writeMAX7320(status&0b11011111);
	} else if(side == LD2) {
		if(state == TRUE)
			writeMAX7320(status|0b01000000);
		else
			writeMAX7320(status&0b10111111);
	}
}

unsigned char readSensor(unsigned char sensor, unsigned char modus) {
	unsigned char ambient = 0;
	unsigned char active = 0;
	unsigned char dgrp;
	unsigned char status;

	writeMAX11604(sensor);	// set channel
	ambient = readMAX11604();
	if(modus == AMBIENT_MODE || sensor == D1 || sensor == D2 || sensor == OS8)
		return ambient;

	if(sensor == OS2 || sensor == OS3)
		dgrp = 2;
	else if(sensor == OS5 || sensor == OS6 || sensor == OS7)
		dgrp = 4;
	else
		dgrp = 1; // OS1 / OS4 and other sensors

	status = readMAX7320();
	writeMAX7320(status|dgrp); // activate sensor and keep status of lights ok
	
	writeMAX11604(sensor);	// set channel
	active = readMAX11604();

	writeMAX7320(status);	// reset status

	if(modus == ACTIVE_MODE)
		return active;

	if(active > ambient)
		return active-ambient;
	else
		return ambient-active;
}

void writeMAX11604(unsigned char channel) {
	// setup of MAX11604
	write1I2C(MAX11604_ADDRESS, 0b01100001|(channel*2));
	delay_us(200);
}

BYTE readMAX11604() {
	unsigned int value;
	value = read1I2C(MAX11604_ADDRESS);
	return (BYTE)value;
}

void writeMAX7320(unsigned char dgrp) {
	// set I/O on channel of MAX7320
	write1I2C(MAX7320_ADDRESS, dgrp);
}

BYTE readMAX7320() {
	unsigned int status;
	status = read1I2C(MAX7320_ADDRESS);
	return (BYTE)status;
}