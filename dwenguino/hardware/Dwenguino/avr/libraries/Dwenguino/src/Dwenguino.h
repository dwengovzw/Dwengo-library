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

#include "DwenguinoLCD.h"

#define PRESSED 0

/**
 * \brief Initialize Dwenguino board
 *
 * Initialization routine for the Dwenguino board. Enables LEDS,
 * enables all switches and sets BUZZER. Additionally the LCD is initialized by this function.
 */
void initDwenguino();

#endif