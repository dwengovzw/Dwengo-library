/**
 * IR
 * part of Dwengo library
 * 
 * Routines for remote control using ifrared RC5 codes
 *
 * Version: 1.0.$Revision: 3933 $
 * Date: $Date: 2011-05-22 17:16:38 +0200 (Sun, 22 May 2011) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

#include "dwengoInterrupt.h"
#include "dwengoRC.h"
#include "dwengoLCD.h"

#define RC_DIR TRISBbits.TRISB4
#define RC_PIN PORTBbits.RB4

#define START_OR_ERROR 0
#define START_BITS 1
#define ONE_BIT_SET 2
#define ONE_BIT_DUMMY 3
#define ZERO_BIT_SET 4
#define ZERO_BIT_DUMMY 5

#define SHORT_MIN 0x1F // everything around 888 us
#define SHORT_MAX 0x32
#define LONG_MIN 0x4A // everything around 1,778 ms
#define LONG_MAX 0x5C 
#define TOO_LONG 0x70 // everything longer than 2,5 ms
#define SHORT_TIME (TMR3H >= SHORT_MIN && TMR3H <= SHORT_MAX)
#define LONG_TIME (TMR3H >= LONG_MIN && TMR3H <= LONG_MAX)
#define TOO_LONG_TIME (TMR3H >= TOO_LONG)
#define NUMBER_OF_ADDRESS_BITS 5
#define NUMBER_OF_COMMAND_BITS 6

volatile int address;
volatile BYTE tmp_address;
volatile int command;
volatile BYTE tmp_command;
volatile BYTE bit_cnt;

volatile unsigned char rcState;

void irISR() {
	BYTE dummy;
	if (PIR2bits.TMR3IF == TRUE) {	// check interrupt flag: from timer 3
		TMR3H = 0x69; // timer 3 reset (saturating timer, set TMR3H slightly lower than TOO_LONG
		TMR3L = 0x00;
		PIR2bits.TMR3IF = FALSE;   // reenable TMR3 interrupt
	} else if(INTCONbits.RBIF == TRUE) { // check interrupt flag: port B interrupt
		INTCONbits.RBIE = TRUE; // disable portb interrupt
		dummy = PORTB; // end mismatch condition		
		// discover state and RC code
		if(TOO_LONG_TIME) {
			rcState = START_OR_ERROR; // if long time ago before an interrupt you come automaticly in start state
		}
		switch(rcState) {
			case START_OR_ERROR:
				if(TOO_LONG_TIME) {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					rcState = START_BITS;
					bit_cnt = 0; // reset bit counter
				}
				break;
			case START_BITS:
				if(SHORT_TIME) {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					if(bit_cnt == 2) {
						rcState = ONE_BIT_SET;
						bit_cnt = -1; // reset bit counter
					} else {
						rcState = START_BITS;
						bit_cnt++;
					}
				} else if(LONG_TIME && bit_cnt == 2) {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					rcState = ZERO_BIT_DUMMY;//ONE_BIT_DUMMY; // start bits have one for toggle bit go to dummy from one
					bit_cnt = 0; // reset bit counter on -1 because toggle bit, not address bit yet
				} else {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					rcState = START_OR_ERROR;
				}
				tmp_address = 0;
				tmp_command = 0;
				break;			
			case ONE_BIT_SET:
				if(SHORT_TIME) {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					if(bit_cnt < NUMBER_OF_ADDRESS_BITS) {
						tmp_address += (1 << (NUMBER_OF_ADDRESS_BITS-1-bit_cnt));
						rcState = ONE_BIT_DUMMY;
					} else if(bit_cnt < (NUMBER_OF_COMMAND_BITS+NUMBER_OF_ADDRESS_BITS-1)) {
						tmp_command += (1 << (NUMBER_OF_COMMAND_BITS+NUMBER_OF_ADDRESS_BITS-1-bit_cnt));
						rcState = ONE_BIT_DUMMY;
					} else if(bit_cnt == (NUMBER_OF_ADDRESS_BITS+NUMBER_OF_COMMAND_BITS-1)) {
						tmp_command += (1 << (NUMBER_OF_COMMAND_BITS+NUMBER_OF_ADDRESS_BITS-1-bit_cnt));
						address = tmp_address; // set address
						command = tmp_command; // set command
						rcState = START_OR_ERROR; // last bit received, go to start
					} else {
						rcState = START_OR_ERROR; // error in code
					}
					bit_cnt++;
				} else {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					rcState = START_OR_ERROR; // from ON_BIT_SET no LONG_TIME that leaves
				}
				break;
			case ONE_BIT_DUMMY:
				if(SHORT_TIME) {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					rcState = ONE_BIT_SET;
				} else if(LONG_TIME) { // set zero bit
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					if(bit_cnt < NUMBER_OF_ADDRESS_BITS) {
						rcState = ZERO_BIT_DUMMY;
					} else if(bit_cnt < (NUMBER_OF_COMMAND_BITS+NUMBER_OF_ADDRESS_BITS-1)) {
						rcState = ZERO_BIT_DUMMY;
					} else if(bit_cnt == (NUMBER_OF_ADDRESS_BITS+NUMBER_OF_COMMAND_BITS-1)) {
						address = tmp_address; // set address
						command = tmp_command; // set command
						rcState = START_OR_ERROR; // last bit received, go to start
					} else {
						rcState = START_OR_ERROR; // error in code
					}
					bit_cnt++;
				} else {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					rcState = START_OR_ERROR;
				}
				break;
			case ZERO_BIT_SET:
				if(SHORT_TIME) {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					if(bit_cnt < NUMBER_OF_ADDRESS_BITS) {
						rcState = ZERO_BIT_DUMMY;
					} else if(bit_cnt < (NUMBER_OF_COMMAND_BITS+NUMBER_OF_ADDRESS_BITS-1)) {
						rcState = ZERO_BIT_DUMMY;
					} else if(bit_cnt == (NUMBER_OF_ADDRESS_BITS+NUMBER_OF_COMMAND_BITS-1)) {
						address = tmp_address; // set address
						command = tmp_command; // set command
						rcState = START_OR_ERROR; // last bit received, go to start
					} else {
						rcState = START_OR_ERROR; // error in code
					}
					bit_cnt++;
				} else {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					rcState = START_OR_ERROR; // from ZERO_BIT_SET no LONG_TIME that leaves
				}
				break;
			case ZERO_BIT_DUMMY:
				if(SHORT_TIME) {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					rcState = ZERO_BIT_SET;
				} else if(LONG_TIME) { // set one bit
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					if(bit_cnt < NUMBER_OF_ADDRESS_BITS) {
						tmp_address += (1 << (NUMBER_OF_ADDRESS_BITS-1-bit_cnt));
						rcState = ONE_BIT_DUMMY;
					} else if(bit_cnt < (NUMBER_OF_COMMAND_BITS+NUMBER_OF_ADDRESS_BITS-1)) {
						tmp_command += (1 << (NUMBER_OF_COMMAND_BITS+NUMBER_OF_ADDRESS_BITS-1-bit_cnt));
						rcState = ONE_BIT_DUMMY;
					} else if(bit_cnt == (NUMBER_OF_ADDRESS_BITS+NUMBER_OF_COMMAND_BITS-1)) {
						tmp_command += (1 << (NUMBER_OF_COMMAND_BITS+NUMBER_OF_ADDRESS_BITS-1-bit_cnt));
						address = tmp_address; // set address
						command = tmp_command; // set command
						rcState = START_OR_ERROR; // last bit received, go to start
					} else {
						rcState = START_OR_ERROR; // error in code
					}
					bit_cnt++;
				} else {
					TMR3H = 0; // reset timer 3
					TMR3L = 0;
					rcState = START_OR_ERROR;
				}
				break;
		}
		// enable interrupt on change and port B interrupt
		INTCONbits.RBIF = FALSE;
		INTCONbits.RBIE = TRUE;
	}
}

void initRC(){
	BYTE dummy;
	initInterrupt();
	registerISR(irISR);

	// Set Timer3 to tick every 83.333ns
	PIR2bits.TMR3IF = 0;	// clear interrupt flag
	RCONbits.IPEN = 1;	// enable priority levels on interrupts
	RCONbits.SBOREN = 0;	// BOR = off
	T3CON = 0b00000001;	// 48Mhz/4, prescaler 1/1, enable timer 3
	IPR2bits.TMR3IP = 1;	// high priority
	PIE2bits.TMR3IE = 1;	// enable timer 3 interrupts
	TMR3H = 0x7C;	// initialize in start position, no code since 2664 us
	TMR3L = 0xE0; // -> 31968 -> 0x7CE0
	
	// enable interrupt on change
	PORTB = 0; // clear port B
	//INTCON2bits.NOT_RBPU = 1; // disable PULL-UPs on port B, switches unusable OPTIE
	//TRISB = 0; // clear port B direction, everything is now configured as output OPTIE
	RC_DIR = 1; // pin RB4 as input
	dummy = PORTB; // end mismatch condition
  	INTCON2bits.RBIP = 1; // set high priority
	INTCONbits.RBIE = 1; // enable port B interrupt
	INTCONbits.RBIF = 0; // clear RB flag

	INTCONbits.GIE = 1;	// set global interrupt enable on
	
	// state and variable initialization
	rcState = START_OR_ERROR;
	bit_cnt = 0;
	
	irISR();
}

int readRC5Command() {
	int tmp = command;
	command = -1;
	return tmp;
}

int readRC5Address() {
	int tmp = address;
	address = -1;
	return tmp;	
}

