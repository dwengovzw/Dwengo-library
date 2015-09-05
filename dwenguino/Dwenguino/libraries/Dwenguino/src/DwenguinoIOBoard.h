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

#ifndef DwenguinoIOBoard_h
#define DwenguinoIOBoard_h

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <Wire.h>

#define IO_ADDR 0x40

// IOBoard
class IOBoard {
    public:
        IOBoard(void);
        IOBoard(unsigned char address);
    
        /**
         * \brief IO-Board initialization function
         *
         * Configure PCA9555 (max 4 IO-boards)
         */
        void init(void);
    
        /**
         * \brief Read from IO-board
         *
         */
        unsigned char readInputs();
    
        /**
         * \brief Write to IO-board
         *
         * @param out byte that defines which relais should be set
         */
        void setOutputs(unsigned char output);
    private:
        unsigned char address;
        unsigned char initialised = false;
};

#endif