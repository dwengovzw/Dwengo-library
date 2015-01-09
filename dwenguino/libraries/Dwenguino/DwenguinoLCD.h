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

#ifndef DwenguinoLCD_h
#define DwenguinoLCD_h

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// following libraries have to be included in main sketch before including Dwenguino.h
// as well to add a compile include path to the library
#include <LiquidCrystal.h>

// LCD
class BufferedLCD : public LiquidCrystal {
public:
	BufferedLCD(uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, volatile uint8_t* data);
    /**
    * \brief Clears the LCD display
    */
	void clear();
    void backlightOn(void);
    void backlightOff(void);
	virtual size_t write(uint8_t);
	void command(uint8_t);

private:
	volatile uint8_t* _dataPort; 
};
extern BufferedLCD dwenguinoLCD;

#endif