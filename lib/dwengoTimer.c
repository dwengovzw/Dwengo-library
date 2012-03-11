 /*
 * Time
 * part of Dwengo library
 *
 * Time sice power up in ms and clock functionality.
 *
 * Version: 1.0.$Revision: 2962 $
 * Date: $Date: 2010-11-12 00:02:39 +0100 (Fri, 12 Nov 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

#include "dwengoTimer.h"

unsigned long time;

unsigned long getTime() {
  return getCycles()/12;  
}

unsigned long getCycles() {
  unsigned long L,H;
  L = TMR0L;
  H = TMR0H;
  return (time << 16)+(H << 8) + L;
}


void timerISR() {
  if (INTCONbits.TMR0IF == 1) {
    INTCONbits.TMR0IF = 0;
    time++;
  }
}

void initTimer(){

  registerISR(timerISR);

  time=0;
  
  // Set Timer0 to tick every ms
  T0CON = 0b10001000;
  TMR0H = 0x00;
  TMR0L = 0x00;
  
  INTCON2bits.TMR0IP = 1;
  INTCONbits.TMR0IF = 0;
  INTCONbits.GIE = 1;
  INTCONbits.TMR0IE = 1;


}