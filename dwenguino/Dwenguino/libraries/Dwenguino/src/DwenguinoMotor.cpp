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

#include <avr/interrupt.h>
#include <Arduino.h>

#include "DwenguinoMotor.h"

DCMotor::DCMotor(uint8_t motor_PWM_pin, uint8_t motor_DIR_pin){
	_motor_PWM = motor_PWM_pin;
	_motor_DIR = motor_DIR_pin;
  currentSpeed = 0;
  init();
}

void DCMotor::init(){
	// intialize Motors
	pinMode(_motor_PWM, OUTPUT);
	pinMode(_motor_DIR, OUTPUT);
	digitalWrite(_motor_PWM, LOW);
	digitalWrite(_motor_DIR, LOW);
}

void DCMotor::setSpeed(int speed){
    if (speed > 255) speed = 255;
    if (speed < -255) speed = -255;
    
    if (speed >= 0) {
        if (currentSpeed < 0) {    // if speed abruptly changes rest 1 ms
            digitalWrite(_motor_DIR, LOW);
            analogWrite(_motor_PWM, 0);
            delayMicroseconds(1000);
        }
        digitalWrite(_motor_DIR, LOW);
        analogWrite(_motor_PWM, speed);
    } else {
        if (currentSpeed > 0) {    // if speed abruptly changes rest 1 ms
            digitalWrite(_motor_DIR, LOW);
            analogWrite(_motor_PWM, 0);
            delayMicroseconds(1000);
        }
        digitalWrite(_motor_DIR, HIGH);
        analogWrite(_motor_PWM, 255+speed);
    }
    currentSpeed = speed;
}

	
