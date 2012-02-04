/*
 * SensorModule
 * part of Dwengo library
 * 
 * Routines to control the Dwengo Robot Sensor Board
 *
 * Version: 1.0.$Revision: 1 $
 * Date: $Date: 2010-05-01 23:09:41 +0200 (za, 01 mei 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

/**
 * \file
 * \brief Dwengo Robot Sensor board routines.
 *
 * Library to control the Dwengo Robot Sensor board.
 */

#ifndef DWENGO_SENSOR_MODULE_H
#define DWENGO_SENSOR_MODULE_H

#include "dwengoBoard.h"

#define DIFF_MODE 		0	// eliminate ambient light
#define AMBIENT_MODE 	1	// read ambient light
#define ACTIVE_MODE		2	// read raw sensor data

#define OS1	0
#define OS2 1
#define OS3 2
#define OS4 3
#define OS5 4
#define OS6 5
#define OS7 6
#define OS8 9
#define D1	7
#define D2	8
#define LD1 0
#define LD2 1

/**
 * \brief Configures the Dwengo Robot Sensor board
 *
 * Initialize the Dwengo Robot Sensor board
 */
void initSensorModule(void);

/**
 * \brief Turn the robot headlights on or off
 *
 * @param side  Choose the headlight LD1 (right) or LD2 (left)
 * @param state TRUE turns on the LED; FALSE turns it off.
 */
void setHeadlights(BYTE side, BYTE state);

/**
 * \brief Read one of the sensors from the module
 *
 * Returns value corresponding with the sensor
 *
 * @param sensor Choose the sensor. Can be OSx, where x is the number of the sensor you want.
 * @param modus  Choose the measuring modus. Can be: DIFF_MODE, AMBIENT_MODE or ACTIVE_MODE

 */
BYTE readSensor(BYTE sensor, BYTE modus); // modus: 0 difference mode, 1 ambient mode, 2 active mode

/**
 * \brief Set the power of the long range distance sensor
 *
 * @param state can be TRUE or FALSE wether you want the long range sensor to be powered or not. Can be used to improve battery life.
 */
void powerLongRange(BYTE state);



#endif // DWENGO_SENSOR_MODULE_H