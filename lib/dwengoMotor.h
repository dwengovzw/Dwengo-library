/*
 * Motor driver
 * part of Dwengo library
 *
 * Control of motor driver on the Dwengo board
 *
 * Version: 1.0.$Revision: 2121 $
 * Date: $Date: 2010-05-01 23:09:41 +0200 (Sat, 01 May 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */
/**
 * \file
 * \brief Motor functionality
 *
 * Control of motor driver on the Dwengo board
 */

#ifndef DWENGO_MOTOR_H
#define DWENGO_MOTOR_H

// Definitions
#define FORWARD     1
#define BACKWARD    0

// Functions
/**
 * \brief Initialize motor functionality
 *
 * Initializes motor functionality for Dwengo board. Includes configuratiation of motor pins to digital outputs,
 * configuration of PWM-modules and Timer2
 */
void initMotor(void);

/**
 * \brief Set speed of motor 1
 *
 * Sets speed of motor 1 in range [-1023,1023]. A delay of a few milliseconds is performed when switching
 * speed abruptly from positive to negative or vice versa.
 * @param speed
 */
void setSpeedMotor1(short speed);
/**
 * \brief Set speed of motor 2
 *
 * Sets speed of motor 2 in range [-1023,1023]. A delay of a few milliseconds is performed when switching
 * speed abruptly from positive to negative or vice versa.
 * @param speed
 */
void setSpeedMotor2(short speed);
/**
 * \brief Stop all motors
 *
 * Sets speed of all motors to zero
 */
void stopMotors(void);


#endif // DWENGO_MOTOR_H