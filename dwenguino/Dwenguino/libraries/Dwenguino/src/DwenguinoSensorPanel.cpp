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

#define MAX11604_ADDRESS B1100101   // Note arduino library uses 7 bit addresses
#define MAX7320_ADDRESS  B1011000

// SensorPanel class
SensorPanel::SensorPanel() {

}

void SensorPanel::init() {
    initialised = true;
    Wire.begin();        // join i2c bus (address optional for master)
    writeMAX7320(B00000000);   // starting with lights out LD1 & LD2  off
    Wire.beginTransmission(MAX11604_ADDRESS);
    Wire.write(B10000010);  // setup MAX11604
    Wire.endTransmission();
}

int SensorPanel::readMAX7320() {
    int current_status = -1;
    if(initialised) {
        unsigned char bytesRead = Wire.requestFrom(MAX7320_ADDRESS, 1);  // ask 1 byte at the address
        if (bytesRead == 1)  current_status = Wire.read(); // read current_status
    }
    return current_status;
}

void SensorPanel::writeMAX7320(unsigned char dgrp) {
    // set I/O on channel of MAX7320
    if(initialised) {
        Wire.beginTransmission(MAX7320_ADDRESS); // transmit to device #4
        Wire.write(dgrp);              // sends one byte
        Wire.endTransmission();
    }
}

void SensorPanel::writeMAX11604(unsigned char channel) {
    if(initialised) {
        Wire.beginTransmission(MAX11604_ADDRESS);
        Wire.write(B01100001|(channel*2));
        Wire.endTransmission();
        delayMicroseconds(200);
    }
}

int SensorPanel::readMAX11604() {
    int current_status = -1;
    if(initialised) {
        unsigned char bytesRead = Wire.requestFrom(MAX11604_ADDRESS, 1);  // ask 1 byte at the address
        if (bytesRead == 1)  current_status = Wire.read(); // read current_status
    }
    return current_status;
}

int SensorPanel::readSensor(unsigned char sensor, unsigned char modus) {
    int ambient = 0;
    int active = 0;
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
    
    writeMAX11604(sensor);  // set channel
    active = readMAX11604();
    
    writeMAX7320(current_status);   // reset status
    
    if(modus == ACTIVE_MODE) return active;
    
    if (active == -1 || ambient == -1) return -1;
    if (active > ambient) return active-ambient;
    else return ambient-active;
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
