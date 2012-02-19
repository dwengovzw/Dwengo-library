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

 /**
  * \file
  * \brief Remote control routines, uses RC5 Philips IR protocol
  *
  * Library to make remote remote control easier
  */

#ifndef DWENGO_RC_H
#define DWENGO_RC_H

#include "dwengoBoard.h"

// Definitions

// Functions


/**
 * \brief Initialize remote control functionality
 *
 * Initializes remote control functionality of the Dwengo board. Configures pin RB4 as digital input,
 * configures timer 3 correctly, enable interrupt-on-change 
 */
void initRC(void);

void irISR();

/**
 * \brief Read latest received RC5 command.
 *
 * Reads latest received RC5 command. If none received since last read -1 is returned
 */
int readRC5Command(void);


/**
 * \brief Read latest received RC5 address.
 *
 * Reads latest received RC5 address. If none received since last read -1 is returned
 */
int readRC5Address(void);

#endif // DWENGO_IR_H