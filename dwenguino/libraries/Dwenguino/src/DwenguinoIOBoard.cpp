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

#include "DwenguinoIOBoard.h"
#include <Wire.h>

// IOBoard class

IOBoard::IOBoard(void) {
    this->address = 0;
}

IOBoard::IOBoard(unsigned char address) {
    this->address = address;
}

void IOBoard::init(void) {
    initialised = true;
    Wire.begin();        // join i2c bus (address optional for master)
    // Configure PCA9555
    Wire.beginTransmission((IO_ADDR|(address<<1))>>1);
    Wire.write(3); // output1
    Wire.write(0);
    Wire.endTransmission();
    delayMicroseconds(200);
    Wire.beginTransmission((IO_ADDR|(address<<1))>>1);
    Wire.write(7); // config1
    Wire.write(0); // output
    Wire.endTransmission();
    delayMicroseconds(200);
    Wire.beginTransmission((IO_ADDR|(address<<1))>>1);
    Wire.write(4); // polarity0
    Wire.write(0xff); // invert_all_inputs
    Wire.endTransmission();
    delayMicroseconds(200);
}

unsigned char IOBoard::readInputs() {
    unsigned char current_status = 0;
    if(!initialised)
        return 0;
    if (address < 4) {
        Wire.beginTransmission((IO_ADDR|(address<<1))>>1);
        Wire.write(0); // input0
        Wire.endTransmission();
        Wire.requestFrom((IO_ADDR|(address<<1))>>1,1);
        if (1<= Wire.available())  // if there's a byte ready...
            current_status = Wire.read(); // ...read current_status
        return current_status;
    }else{
        return current_status;
    }
}

void IOBoard::setOutputs(unsigned char output) {
    if(initialised) {
        if (address < 4) {
            Wire.beginTransmission((IO_ADDR|(address<<1))>>1);
            Wire.write(3); // output1
            Wire.write(output);
            Wire.endTransmission();
        }
    }
}
