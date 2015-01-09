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

#include "Dwenguino.h"

BufferedLCD dwenguinoLCD(LCD_RS, LCD_RW, LCD_E, LCD_D0, LCD_D1, LCD_D2, LCD_D3, LCD_D4, LCD_D5, LCD_D6, LCD_D7, &LEDS);

void initDwenguino(){

	// initialize LCD
	digitalWrite(LCD_BL, HIGH);	// Enable backlight
	dwenguinoLCD.begin(16,2);
    dwenguinoLCD.clear();

	// initialize Leds
    LEDS_DIR = 0xFF;
    LEDS = 0;

	// initialize Buttons
	pinMode(SW_W, INPUT_PULLUP);
	pinMode(SW_S, INPUT_PULLUP);
	pinMode(SW_E, INPUT_PULLUP);
	pinMode(SW_N, INPUT_PULLUP);
	pinMode(SW_C, INPUT_PULLUP);

	// initialize Buzzer
  	pinMode(BUZZER, OUTPUT);
}
