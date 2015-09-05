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

#include "DwenguinoLCD.h"

// Make custom dwenguinoLCD class which inherits from the original LiquidCrystal library
// This class termporary stores the values on the leds and restores the LEDS after a lcd command, since the lcd pins and led pins are multiplexed
BufferedLCD::BufferedLCD(uint8_t rs, uint8_t rw, uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, volatile uint8_t* data)
	:LiquidCrystal(rs,rw,enable,d0,d1,d2,d3,d4,d5,d6,d7)
{
	_dataPort = data;
}


void BufferedLCD::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void BufferedLCD::backlightOn() {
    digitalWrite(LCD_BL, HIGH);	// Enable backlight
}

void BufferedLCD::backlightOff() {
    digitalWrite(LCD_BL, LOW);	// Disable backlight
}

inline void BufferedLCD::command(uint8_t value) {
  uint8_t temp = *_dataPort; 
  LiquidCrystal::command(value);
  *_dataPort = temp;
}

inline size_t BufferedLCD::write(uint8_t value) {
  uint8_t temp = *_dataPort; 
  size_t returnVal = LiquidCrystal::write(value);
  *_dataPort = temp;
  return returnVal;
}