/**
 * Servo
 * part of Dwengo library
 * 
 * Routines to control servos
 *
 * Version: 1.0.$Revision: 2590 $
 * Date: $Date: 2010-08-17 21:13:05 +0200 (Tue, 17 Aug 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

#include "dwengoInterrupt.h"
#include "dwengoServo.h"


volatile unsigned int minTimeServo[2];
volatile unsigned int maxTimeServo[2];

volatile unsigned int highTimerSetting[2];
volatile unsigned int lowTimerSetting[2];

volatile unsigned char nextState;

void servoISR() {
  if (PIR1bits.TMR1IF == TRUE) {            // check interrupt flag: from timer?
	if (nextState == 0) {
        TMR1H = (highTimerSetting[0] & 0xFF00) >> 8;
        TMR1L = highTimerSetting[0] & 0x00FF;
	    SERVO1 = 1;
		SERVO2 = 0;
		nextState = 1;
	} else if (nextState == 1) {
        TMR1H = (lowTimerSetting[0] & 0xFF00) >> 8;
        TMR1L = lowTimerSetting[0] & 0x00FF;
	    SERVO1 = 0;
		SERVO2 = 0;
		nextState = 2;
	} else if (nextState == 2) {
        TMR1H = (highTimerSetting[1] & 0xFF00) >> 8;
        TMR1L = highTimerSetting[1] & 0x00FF;
	    SERVO1 = 0;
		SERVO2 = 1;
		nextState = 3;
	} else if (nextState == 3) {
        TMR1H = (lowTimerSetting[1] & 0xFF00) >> 8;
        TMR1L = lowTimerSetting[1] & 0x00FF;
	    SERVO1 = 0;
		SERVO2 = 0;
		nextState = 0;
	}    
    PIR1bits.TMR1IF = FALSE;   // reenable TMR1 interrupt
  }
}

void initServo(){

  initInterrupt();
  registerISR(servoISR);

  // Set Timer1 to tick every 83.333ns
  PIR1bits.TMR1IF = 0;
  RCONbits.IPEN = 1;
  RCONbits.SBOREN = 0;   // BOR = off
  T1CON = 0b00110001;    // 48MHz/4, prescaler 1/8
  INTCONbits.GIE = 1;
  IPR1bits.TMR1IP = 1;
  PIE1bits.TMR1IE = 1;

  // Set servo pins as outputs
  SERVO1_DIR = 0;
  SERVO2_DIR = 0;
  LEDS_DIR   = 0x00;

  // Initialize the servo pins to 0
  SERVO1 = 0;
  SERVO2 = 0;

  // Initialize servo range
  setServoRange(1, MIN_TIME_SERVO, MAX_TIME_SERVO);
  setServoRange(2, MIN_TIME_SERVO, MAX_TIME_SERVO);

  // Initialize the servo positions
  setPosition(1,0);
  setPosition(2,0);

  nextState = 0;
  servoISR();
}

// void initServos(BYTE nb_servos, int bereik);
void setServoRange(BYTE servo, int minTime, int maxTime) {
	INTCONbits.GIE = 0;
	minTimeServo[servo-1] = minTime;
	maxTimeServo[servo-1] = maxTime;
	INTCONbits.GIE = 1;
}

void setPosition(BYTE servo, BYTE position){
	unsigned int highTimeTicks;
	INTCONbits.GIE = 0;
    highTimeTicks = minTimeServo[servo-1] + ( (unsigned long) position * (maxTimeServo[servo-1] - minTimeServo[servo-1]))/256;
	
	highTimerSetting[servo-1] = 0xFFFF - highTimeTicks;
	lowTimerSetting[servo-1]  = 0XFFFF - (PERIOD - highTimeTicks);
	INTCONbits.GIE = 1;

}

// void stopServos();
