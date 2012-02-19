/*
 * Servo
 * part of Dwengo library
 * 
 * Routines to control servos
 *
 * Version: 1.0.$Revision: 3982 $
 * Date: $Date: 2011-06-05 16:23:01 +0200 (Sun, 05 Jun 2011) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

/**
 * \file
 * \brief Servo motor routines.
 *
 * Library to control servo motors.
 */

#ifndef DWENGO_SERVO_H
#define DWENGO_SERVO_H

#include "dwengoBoard.h"

// Definitions
#define MIN_TIME_SERVO 999		// 0.7 ms
#define MAX_TIME_SERVO 3449		// 2.3 ms
#define PERIOD         15000

// Functions


/**
 * \brief Initialize servo functionality
 *
 * This routine initializes servo functionality of the Dwengo board. It configures the servo pins as 
 * digital outputs. It also configures timer1 and dispatches the servo interrupt service routine 
 * using the dwengo interrupt librairy. The minimum and maximum pulse widths for both servos are set
 * to 0.7 ms and 2.3 ms by default.
 */
void initServo(void);

void servoISR();

/**
 * \brief Set the puls width rage of a servo.
 *
 * This routine sets the minimum and maximum puls width for a servo. Both times are expressed in 
 * units of 83.33ns. The initServos routine by default sets these times to 0.7 ms and 2.3 ms 
 * respectively.
 * 
 * @param servo Number specifing the servo: 1 for SERVO1 and 2 for SERVO2
 * @param minTime Minimum pulse width expressed in units of 83.33ns.
 * @param maxTime Maximum pulse width expressed in units of 83.33ns.
 */
void setServoRange(BYTE servo, int minTime, int maxTime);

/**
 * \brief Set the possition of a servo.
 *
 * Sets the possition of a servo. The possition is expressed as a number between 0 and 255.
 * Possition 0 corresponds with a pulse width equal to the minimum pulse width. Possition 255
 * corresponds to a pulse width equal to the maximum pulse width.
 *
 * @param servo Number specifing the servo: 1 for SERVO1 and 2 for SERVO2
 * @param position Number that specifies the possition of the servo. 0 corresponds to the minimal
 *                 pulse width 255 corresponds to the maximal pulse width.
 */
void setPosition(BYTE servo, unsigned char position);

// void stopServos();

#endif // DWENGO_SERVO_H