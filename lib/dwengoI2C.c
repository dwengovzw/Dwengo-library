#include "dwengoBoard.h"
#include "dwengoI2C.h"

#define MAX_DELAY 0xEFFF // stop blocking communication

/* I2C low-level functions */

void startI2C(unsigned char addr, char rw) {
	unsigned int block_cnt;
	SSPCON2bits.SEN = 1;  // start
	block_cnt = 0;
  	while(SSPCON2bits.SEN && block_cnt++<MAX_DELAY); // wait
  	SSPCON2bits.ACKDT = 0;

  	PIR1bits.SSPIF = 0;
  	SSPBUF = addr | rw;
  	block_cnt = 0;
	while(!PIR1bits.SSPIF && block_cnt++<MAX_DELAY);
  	PIR1bits.SSPIF = 0;
}

void writeI2C(unsigned char val) {
	unsigned int block_cnt = 0;
	SSPBUF = val;
  	while(!PIR1bits.SSPIF && block_cnt++<MAX_DELAY);
  	PIR1bits.SSPIF = 0;
}

unsigned char readI2C(char ack) {
	unsigned int block_cnt = 0;
    unsigned char val;

  	SSPCON2bits.RCEN = 1; // receive data
  	while(!SSPSTATbits.BF && block_cnt++<MAX_DELAY); // wait for data
  	val = SSPBUF; // read data

  	if (ack) {
    	SSPCON2bits.ACKEN = 1;  // send ACK
		block_cnt = 0;
    	while(SSPCON2bits.ACKEN && block_cnt++<MAX_DELAY);
  	}
  	return val;
}

void stopI2C() {
	unsigned int block_cnt = 0;
  	SSPCON2bits.ACKDT = 1;  // send not ACK
  	SSPCON2bits.PEN = 1;    // stop
  	while(SSPCON2bits.PEN && block_cnt++<MAX_DELAY);   // wait
}

/* high-level I2C functions */

void write1I2C(unsigned char addr, unsigned char val) {
	startI2C(addr, I2C_WRITE);
  	writeI2C(val);
  	stopI2C();
}

void write2I2C(unsigned char addr, unsigned char val1, unsigned char val2) {
  	startI2C(addr, I2C_WRITE);
  	writeI2C(val1);
  	writeI2C(val2);
  	stopI2C();
}

int read1I2C(unsigned char addr) {
    unsigned char val;

    startI2C(addr, I2C_READ);
    val = readI2C(0);
    return val;
}

int read2I2C(unsigned char addr) {
  	unsigned char val_high, val_low;
    startI2C(addr, I2C_READ);
    val_high = readI2C(1);
  	val_low = readI2C(0);
    stopI2C();
  	return ((unsigned int)val_high << 8L) | val_low;
}


void initI2C() {
  	unsigned char dummy;

  	// configuration of I2C
  	TRISBbits.TRISB0 = 1; // RB0 and RB1 as inputs
  	TRISBbits.TRISB1 = 1;

  	SSPSTATbits.SMP = 1;  // disable SLEW RATE for 100 kHz
  	SSPADD = 119;     // set baud to 100 kHz
  	SSPCON1 = 0b00101000;   // set SSPEN bit: scl and sda are open-drain now, MASTER, clock = (Fosc/4)*(SSPADD+1)
  	SSPCON2 = 0; // clear all
  	dummy = SSPBUF; // dummy read
}
