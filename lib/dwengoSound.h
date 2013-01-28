/**
 * dwengoSound.h
 *
 * Routines to control speakers. Headers file
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

#ifndef DWENGO_SOUND_H
#define DWENGO_SOUND_H

#include "dwengoBoard.h"
// #include "dwengoNotes.h"

#define SOUND1_0            PORTCbits.RC2
#define SOUND1_0_DIR        TRISCbits.TRISC2
#define SOUND1_1            PORTAbits.RA4
#define SOUND1_1_DIR        TRISAbits.TRISA4
#define SOUND2_0            PORTCbits.RC1
#define SOUND2_0_DIR        TRISCbits.TRISC1
#define SOUND2_1            PORTBbits.RB3
#define SOUND2_1_DIR        TRISBbits.TRISB3

// Global variables
extern unsigned char BEEP;
extern unsigned char preloadTMR1L;
extern unsigned char preloadTMR1H;
extern unsigned short TMR0Count;
extern unsigned char BEAT_SPEED;

// Functions
/**
 * \brief Initialize sound functionality
 *
 * This routine initializes sound functionality of the Dwengo board. It
 * configures the motor pins as digital outputs. It also configures timer0 and
 * timer1 and dispatches the sound interrupt service routine
 * using the dwengo interrupt librairy.
 */
void initSound (void);

void soundISR ();

/**
 * \brief Plays a musical note
 *
 * TODO: write help
 */
void playNote (unsigned int note, unsigned int duration);

#endif // DWENGO_SOUND_H
