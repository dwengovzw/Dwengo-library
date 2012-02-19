 /*
 * Timer
 * part of Dwengo library
 *
 * Time sice power up in ms and clock functionality.
 *
 * Version: 1.0.$Revision: 2962 $
 * Date: $Date: 2010-11-12 00:02:39 +0100 (Fri, 12 Nov 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */
 
 /**
  * \file
  * \brief Timer routines
  *
  * Time since power up in microseconds.
  */

#ifndef DWENGO_TIMER_H
#define DWENGO_TIMER_H

#include "dwengoBoard.h"
#include "dwengoInterrupt.h"
#include "dwengoLCD.h"

/**
 * \brief Get the time since power up.
 *
 * This function returns the number of microseconds since initTimer().
 *
 */
unsigned long getTime(void);

/**
 * \brief Get the time since power up.
 *
 * This function returns the number of cycles since initTimer().
 *
 */
unsigned long getCycles(void);

/**
 * \brief Initialize the timer
 *
 * This routine initializes the timer functionality. It configures timer0 
 * and dispatches the timer interrupt service routine using the Dwengo interrupt library.
 */
void initTimer(void);

void timeISR(void);


#endif // DWENGO_TIMER_H