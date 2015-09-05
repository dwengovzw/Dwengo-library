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

#include "DwenguinoSensorPanel.h"
#include <Wire.h>

// SensorPanel class
SensorPanel::SensorPanel() {

}

void SensorPanel::init() {
    initialised = true;
    Wire.begin();        // join i2c bus (address optional for master)
    writeMAX7320(B00000000);   // starting with lights out LD1 & LD2  off
    writeMAX11604(B10000010);
}

unsigned char SensorPanel::readMAX7320() {
    unsigned int current_status = 0;
    if(initialised) {
        Wire.requestFrom(B1011000,1);  // ask 1 byte at the address
        delayMicroseconds(200);
        //Serial.print(Wire.available());
        if (1<= Wire.available())  // if there's a byte ready...
            current_status = Wire.read(); // ...read current_status
    }
    return (unsigned char)current_status;
}

void SensorPanel::writeMAX7320(unsigned char dgrp) {
    // set I/O on channel of MAX7320
    if(initialised) {
        Wire.beginTransmission(B1011000); // transmit to device #4
        Wire.write(dgrp);              // sends one byte
        Wire.endTransmission();
    }
}

void SensorPanel::writeMAX11604(unsigned char channel) {
    // setup of MAX11604
    // setup of MAX11604
    if(initialised) {
        Wire.beginTransmission(B1100101);
        Wire.write(B01100001|(channel*2));
        Wire.endTransmission();
        delayMicroseconds(200);
    }
}

unsigned char SensorPanel::readMAX11604() {
    unsigned int current_status = 0;
    if(initialised) {
        Wire.requestFrom(B1100101,1);  // ask 1 byte at the address
        //Serial.print(Wire.available());
        if (1<= Wire.available())  // if there's a byte ready...
            current_status = Wire.read(); // ...read current_status
    }
    return (unsigned char)current_status;
}

unsigned char SensorPanel::readSensor(unsigned char sensor, unsigned char modus) {
    unsigned char ambient = 0;
    unsigned char active = 0;
    unsigned char dgrp;
    unsigned char current_status;
    
    writeMAX11604(sensor);
    ambient = readMAX11604();
    if(modus == AMBIENT_MODE || sensor == D1 || sensor == D2 || sensor == OS8)
        return ambient;
    if(sensor == OS2 || sensor == OS3)
        dgrp = 2;
    else if(sensor == OS5 || sensor == OS6 || sensor == OS7)
        dgrp = 4;
    else
        dgrp = 1; // OS1 / OS4 and other sensors
    
    current_status = readMAX7320();
    writeMAX7320(current_status|dgrp); // activate sensor and keep status of lights ok
    
    writeMAX11604(sensor);	// set channel
    active = readMAX11604();
    
    writeMAX7320(current_status);	// reset status
    
    if(modus == ACTIVE_MODE)
        return active;
    
    if(active > ambient)
        return active-ambient;
    else
        return ambient-active;
}

void SensorPanel::powerLongRange(unsigned char state) {
    unsigned char current_status;
    current_status = readMAX7320();
    if(state == 1)
        writeMAX7320(current_status&B11110111); // long range active (dgrp3 must be 0)
    else
        writeMAX7320(current_status|B00001000); // long range unactive
}

void SensorPanel::setHeadlights(boolean side,unsigned char state){
    unsigned char current_status;
    current_status = readMAX7320();
    if(side == LD1) {
        if(state == true)
            writeMAX7320(current_status|B00100000);              // sends one byte
        else
            writeMAX7320(current_status&B11011111);              // sends one byte
    }
    else if(side == LD2) {
        if(state == true)
            writeMAX7320(current_status|B01000000);               // LED 2 OFF
        else
            writeMAX7320( current_status&B10111111);              // sends one byte
    }
}
