 /*
 * Clock
 * part of Dwengo library
 *
 * Functionality for keeping time and showing time on the LCD.
 *
 * Version: 1.0.$Revision: 2950 $
 * Date: $Date: 2010-11-11 14:31:50 +0100 (Thu, 11 Nov 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

#include "dwengoClock.h"

char hours;
char mins;
char secs;

char visable;


char clockLine;
char clockChar;

void printClockToLCD(char l, char p) {
  printCharToLCD('0'+(hours/10), l, p);
  appendCharToLCD('0'+(hours%10));
  appendCharToLCD(':');
  appendCharToLCD('0'+(mins/10));
  appendCharToLCD('0'+(mins%10));
  appendCharToLCD(':');
  appendCharToLCD('0'+(secs/10));
  appendCharToLCD('0'+(secs%10));
}


char getHours() {
  return hours;   
}

char getMins() {
  return mins;   
}

char getSecs() {
  return secs;   
}

void setClock(char h, char m, char s) {
  hours = h;
  mins  = m;
  secs  = s;
}

void setClockPosition (char l, char p) {
    clockLine = l;
    clockChar = p;
}

void hideClock() {
  visable = 0;
  setCursorLCD(clockLine, clockChar);
  appendStringToLCD("        ");
}

void showClock() {
    visable = TRUE;
    printClockToLCD(clockLine, clockChar);
}

void clockISR() {
  if (INTCONbits.TMR0IF == 1) {
    TMR0H = 0x48;
    TMR0L = 0xe5;
    INTCONbits.TMR0IF = 0;
        
    //Update clock.
    secs++;
    if (secs == 60) {
      secs=0;
      mins++;
      if (mins == 60) {
        mins = 0;
        hours++;
        if (hours == 24) {
          hours = 0;
        }
      }
    }
    
    
    if (visable==TRUE) {
      printClockToLCD(clockLine, clockChar);
    }
    
  }

}

void initClock(){

  registerISR(clockISR);

  // Set Timer0 to tick every second
  T0CON = 0b10000111;
  TMR0H = 0x48;
  TMR0L = 0xe5;
  
  INTCON2bits.TMR0IP = 1;
  INTCONbits.TMR0IF = 0;
  INTCONbits.GIE = 1;
  INTCONbits.TMR0IE = 1;
  
  visable = FALSE;

}