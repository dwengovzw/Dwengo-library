/**
 * dwengoSound.c
 *
 * Routines to control speakers
 *
 * Version: 1.0.0
 * Date:  Monday, November 26 2012
 *
 * Copyright (C) 2012 - Juan Pablo Carbajal
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "dwengoInterrupt.h"
#include "dwengoSound.h"

// Global variables
unsigned char BEEP;
unsigned char preloadTMR1L;
unsigned char preloadTMR1H;
unsigned short TMR0Count;
unsigned char BEAT_SPEED;

// Interrupt service routine
void soundISR () {

  if (INTCONbits.TMR0IF == 1) {   // TIMER0 interrupt?

    TMR0L = BEAT_SPEED;

    if (TMR0Count) TMR0Count--;

    INTCONbits.TMR0IF = 0;        // Reenable TIMER0 interrupt
  }

  if (PIR1bits.TMR1IF == 1) {   // TIMER1 interrupt?
      if (BEEP)
        SOUND1_0 = !SOUND1_0;
      else
        SOUND1_0 = 0;

      TMR1H = preloadTMR1H;
      TMR1L = preloadTMR1L;
      PIR1bits.TMR1IF = 0;        // Reenable TIMER1 interrupt
  }

}

// Initialize Sound
void initSound (void){

  initInterrupt ();
  registerISR (soundISR);

  /* Enable interrups */
  INTCONbits.GIE  = 1;
  INTCONbits.PEIE = 1;

 /*
  * Configure Timer 0 8-bit internal Clk
  * Initialise Timer 0 from bit 0 to bit 7
  */

 /*
  * Initialise prescaler to 256
  * 111 = 1:256 Prescale value
  * 110 = 1:128 Prescale value
  * 101 = 1:64   Prescale value
  * 100 = 1:32   Prescale value
  * 011 = 1:16   Prescale value
  * 010 = 1:8     Prescale value
  * 001 = 1:4     Prescale value
  * 000 = 1:2     Prescale value
  */
  T0CONbits.T0PS0 = 1;
  T0CONbits.T0PS1 = 1;
  T0CONbits.T0PS2 = 1;
  // Activate prescaler
  T0CONbits.PSA     = 0;
  // Source edge select (not really important)
  T0CONbits.T0SE    = 1;
  // Select the source to internal clock
  T0CONbits.T0CS    = 0;
  // Configure TIMER0 to be 8-bit
  T0CONbits.T08BIT  = 1;
  // Activate timer
  T0CONbits.TMR0ON  = 1;
  /* End configuration*/
  /********************/
  // Clear TMR0 Flag, ready for use
  INTCONbits.TMR0IF = 0;

  // Allow TIMER0 interrupts
  INTCONbits.TMR0IE = 1;

 /*
  * Configure Timer 1
  * Initialise Timer 0 from bit 7 to bit 0
  */

  // Write the register in two 8-bit operations
  // TODO one 16-bit write?
  T1CONbits.RD16 = 0;

  // Timer1 System Clock Status bit
  // Use the anther source
  T1CONbits.T1RUN = 0;
 /*
  * Initialise prescaler to 8
  * 11 = 1:8 Prescale value
  * 10 = 1:4 Prescale value
  * 01 = 1:2 Prescale value
  * 00 = 1:1 Prescale value
  */
  T1CONbits.T1CKPS0 = 1;
  T1CONbits.T1CKPS1 = 1;
  // Disable internal oscillator
  T1CONbits.T1OSCEN = 0;
  // Timer1 External Clock Input Synchronization Select bit (Ignored when TMR1CS == 0)
  T1CONbits.T1SYNC  = 0;
  // Select the source to internal clock (Fosc/4)
  T1CONbits.TMR1CS  = 0;
  // Activate timer
  T1CONbits.TMR1ON  = 1;
  /* End configuration*/

  // Clear TMR0 Flag, ready for use
  PIR1bits.TMR1IF = 0;

  // Allow TIMER1 interrupts
  PIE1bits.TMR1IE = 1;

  // Initialise sound pin
  SOUND1_0_DIR = 0;  // Set RC2 as output
  SOUND1_1_DIR = 0;  // Set RC2 as output
  SOUND1_0     = 0;  // Initialise with 0
  SOUND1_1     = 0;  // Initialise with 0

  SOUND2_0_DIR = 0;  // Set RC1 as output
  SOUND2_1_DIR = 0;  // Set RC1 as output
  SOUND2_0     = 0;  // Initialise with 0
  SOUND2_1     = 0;  // Initialise with 0

  TMR0Count = 0;
  BEEP = 0;
  soundISR ();
}

void playNote(unsigned int note, unsigned int duration)
{
    /* Wait until last note has played */
    while(TMR0Count) { };
    BEEP = 0;

    /* Process New Note Frequency */
    if (note) {
        note = ~note;
        preloadTMR1L = (note & 0xFF);
        preloadTMR1H = ((note & 0xFF00) >> 8);
    }

    /* Process Note Duration */
    TMR0Count = 255/(duration & 0x7F);

    /* If duration is 1.5x add .5 to duration */
    if (duration & 0x80) TMR0Count = (TMR0Count + (TMR0Count >> 1));

    if (note) BEEP = 1;
}
