#include "dwengoBoard.h"

void initBoard(void) {

	// if lcd display is connected (default)
#ifndef NO_LCD
	// init the lcd display
	initLCD();
	clearLCD();
#endif
	
	// set buttons as inputs
	SW_C_DIR = INPUT;
	SW_N_DIR = INPUT;
	SW_S_DIR = INPUT;
	SW_W_DIR = INPUT;
	SW_E_DIR = INPUT;

	// enable pull-up resistors of port B (buttons)
	INTCON2bits.NOT_RBPU = 0;  
	
	// set led pins as output
	LEDS_DIR = 0;  // led pins as outputs
	LEDS = 0;   // switch leds off	
}

void setLedRegister(BYTE led) {
	LEDS = led;
}

void setLed(BYTE position) {
  switch (position) {
    case 0: LED0 = TRUE; break;
    case 1: LED1 = TRUE; break;
    case 2: LED2 = TRUE; break;
    case 3: LED3 = TRUE; break;
    case 4: LED4 = TRUE; break;
    case 5: LED5 = TRUE; break;
    case 6: LED6 = TRUE; break;
    case 7: LED7 = TRUE; break;
  }
}

void clearLed(BYTE position) {
  switch (position) {
    case 0: LED0 = FALSE; break;
    case 1: LED1 = FALSE; break;
    case 2: LED2 = FALSE; break;
    case 3: LED3 = FALSE; break;
    case 4: LED4 = FALSE; break;
    case 5: LED5 = FALSE; break;
    case 6: LED6 = FALSE; break;
    case 7: LED7 = FALSE; break;
  }
}

void setLedPattern(BYTE ld7, BYTE ld6, BYTE ld5, BYTE ld4, BYTE ld3, BYTE ld2, BYTE ld1, BYTE ld0) {
	LED0 = ld0;
	LED1 = ld1;
	LED2 = ld2;
	LED3 = ld3;
	LED4 = ld4;
	LED5 = ld5;
	LED6 = ld6;
	LED7 = ld7;
}
