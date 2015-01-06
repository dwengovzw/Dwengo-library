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

#ifndef Dwenguino_h
#define Dwenguino_h

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// following libraries have to be included in main sketch before including Dwenguino.h
// as well to add a compile include path to the library
#include <LiquidCrystal.h>
#include <Wire.h>

// Say which 16 bit timers can be used and in what order
//#define _useTimer1  0
//#define _useTimer3  1
//typedef enum { _timer1, _timer3, _Nbr_16timers } timer16_Dwenguino_Sequence_t;

// Defines
#define DIFF_MODE 	0	// eliminate ambient light
#define AMBIENT_MODE 	1	// read ambient light
#define ACTIVE_MODE	2	// read raw sensor data

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

#define IO_ADDR 0x40

// Classes
// LCD
class BufferedLCD : public LiquidCrystal {
public:
	BufferedLCD(uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, volatile uint8_t* data);

	void clear();
	virtual size_t write(uint8_t);
	void command(uint8_t);

private:
	volatile uint8_t* _dataPort; 
};
extern BufferedLCD dwenguinoLCD;

// Motors
class Motors {
public:
	Motors(uint8_t motor1_PWM, uint8_t motor1_DIR, uint8_t motor2_PWM, uint8_t motor2_DIR);
	void init(void);
	void setSpeedMotor1(int speed);
	void setSpeedMotor2(int speed);
	void stopMotors();
private:
	uint8_t _motor1_PWM, _motor1_DIR, _motor2_PWM, _motor2_DIR;
    int currentSpeed1;
    int currentSpeed2;
};
//extern Motors dwenguinoMotors;

// SensorPanel
class SensorPanel {
public:
    SensorPanel();
    void init(void);
    void setHeadlights(boolean, unsigned char);
    unsigned char readSensor(unsigned char, unsigned char); // modus: 0 difference mode, 1 ambient mode, 2 active mode
    void powerLongRange(unsigned char);
    
private:
    unsigned char readMAX7320(void);
    void writeMAX7320(unsigned char);
    unsigned char readMAX11604(void);
    void writeMAX11604(unsigned char);
    unsigned char initialised = false;
};
//extern SensorPanel dwenguinoSensorPanel;

// IOBoard
class IOBoard {
    public:
        IOBoard(void);
        IOBoard(unsigned char address);
        void init(void);
        unsigned char readInputs_addr(unsigned char address);
        void setOutputs_addr(unsigned char address, unsigned char output);
        unsigned char readInputs();
        void setOutputs(unsigned char out);
    private:
        unsigned char address;
        unsigned char initialised = false;
};

// Function prototypes
void initDwenguino();

#endif