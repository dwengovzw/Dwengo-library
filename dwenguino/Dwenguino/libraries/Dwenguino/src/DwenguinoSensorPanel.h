/* ---------------------------------------------------------------------------
    Dwenguino Library - v2.0 
    
    Created on Dec 20 2014 by Jelle Roets and Francis wyffels from Dwengo vzw (www.dwengo.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
* --------------------------------------------------------------------------- */

#ifndef DwenguinoSensorPanel_h
#define DwenguinoSensorPanel_h

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// following libraries have to be included in main sketch before including Dwenguino.h
// as well to add a compile include path to the library
#include <Wire.h>

// Defines
#define DIFF_MODE   0   // eliminate ambient light
#define AMBIENT_MODE    1   // read ambient light
#define ACTIVE_MODE 2   // read raw sensor data

#define OS1   0
#define OS2   1
#define OS3   2
#define OS4   3
#define OS5   4
#define OS6   5
#define OS7   6
#define OS8   9
#define D1    7
#define D2    8
#define LD1   0
#define LD2   1

// SensorPanel
class SensorPanel {
public:
    SensorPanel();
    
    /**
     * \brief Configures the Dwengo Robot Sensor board
     *
     * Initialize the Dwengo Robot Sensor board
     */
    void init(void);
    
    /**
     * \brief Turn the robot headlights on or off
     *
     * @param side  Choose the headlight LD1 (right) or LD2 (left)
     * @param state true turns on the LED; false turns it off.
     */
    void setHeadlights(boolean, unsigned char);
    
    /**
     * \brief Read one of the sensors from the module
     *
     * Returns value corresponding with the sensor, returns -1 if an error occured
     *
     * @param sensor Choose the sensor. Can be OSx, where x is the number of the sensor you want.
     * @param modus  Choose the measuring modus. Can be: DIFF_MODE, AMBIENT_MODE or ACTIVE_MODE
     
     */
    int readSensor(unsigned char, unsigned char); // modus: 0 difference mode, 1 ambient mode, 2 active mode
    
    /**
     * \brief Set the power of the long range distance sensor
     *
     * @param state can be true or false wether you want the long range sensor to be powered or not. Can be used to improve battery life.
     */
    void powerLongRange(unsigned char);
    
private:
    int readMAX7320(void);
    void writeMAX7320(unsigned char);
    int readMAX11604(void);
    void writeMAX11604(unsigned char);
    unsigned char initialised = false;
};

#endif