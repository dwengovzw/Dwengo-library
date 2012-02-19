 /*
 * Time
 * part of Dwengo library
 *
 * Functionality for keeping time and showing time on the LCD.
 *
 * Version: 1.0.$Revision: 2950 $
 * Date: $Date: 2010-11-11 14:31:50 +0100 (Thu, 11 Nov 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */
 
 /**
  * \file
  * \brief Clock functionality
  *
  * Functionality for keeping time and showing time on the LCD.
  *
  */

#ifndef DWENGO_CLOCK_H
#define DWENGO_CLOCK_H

#include "dwengoBoard.h"
#include "dwengoInterrupt.h"
#include "dwengoLCD.h"

/**
 * \brief Get the clocks hours.
 *
 * Returns the number of hours on the clock.
 *
 */
char getHours(void);

/**
 * \brief Get the clocks minutes.
 *
 * Returns the number of minutes on the clock.
 *
 */
char getMins(void);

/**
 * \brief Get the clocks seconds.
 *
 * Returns the number of seconds on the clock.
 *
 */
char getSecs(void);


/**
 * \brief Set the clock.
 *
 * Sets the clock's hours, minutes and seconds.
 *
 * @param h The new value for the clocks hours.
 * @param m The new value for the clocks minutes.
 * @param s The new value for the clocks seconds.
 */
void setClock(char h, char m, char s);


/**
 * \brief Set the clock position.
 *
 * Sets the position at which the clock is shown on the LCD.
 *
 * @param l The line number (0 or 1).
 * @param p The horizontal position (0 to 15).
 */
void setClockPosition (char l, char p);

/**
 * \brief Hide the clock.
 *
 * Hides the clock.
 */
void hideClock(void);

/**
 * \brief Show the clock.
 *
 * Shows the clock.
 */
void showClock(void);

void clockISR(void);

/**
 * \brief Initialize the clock
 *
 * This routine initializes the clock functionality. It configures timer0 
 * and dispatches the clock interrupt service routine using the Dwengo interrupt library. The clockISR
 * is called every second to update the time. If the clock is visible, the routine also will print
 * the updated time on the LCD in the position set by the setClockPosition().
 */
void initClock(void);


#endif // DWENGO_CLOCK_H