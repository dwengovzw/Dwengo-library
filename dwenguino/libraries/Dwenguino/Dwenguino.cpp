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
//Motors dwenguinoMotors(MOTOR_1_0, MOTOR_1_1, MOTOR_1_0, MOTOR_1_1);

void initDwenguino(){

	// initialize LCD
	digitalWrite(LCD_BL, HIGH);	// Enable backlight
	dwenguinoLCD.begin(16,2);

	// initialize Leds
    LEDS_DIR = 0xFF;
    LEDS = 0;

	// initialize Buttons
	pinMode(SW_W, INPUT_PULLUP);
	pinMode(SW_S, INPUT_PULLUP);
	pinMode(SW_E, INPUT_PULLUP);
	pinMode(SW_N, INPUT_PULLUP);
	pinMode(SW_C, INPUT_PULLUP);
  
	// initialize Servo's 
	//servo1.attach(SERVO_1);  
	//servo2.attach(SERVO_2);

	// initialize Buzzer
  	pinMode(BUZZER, OUTPUT);
}

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

// Motor class

Motors::Motors(uint8_t motor1_PWM, uint8_t motor1_DIR, uint8_t motor2_PWM, uint8_t motor2_DIR){
	_motor1_PWM = motor1_PWM;
	_motor1_DIR = motor1_DIR;
	_motor2_PWM = motor2_PWM;
	_motor2_DIR = motor2_DIR;
    currentSpeed1 = 0;
    currentSpeed2 = 0;
    init();
}

void Motors::init(){
	// intialize Motors
	pinMode(_motor1_PWM, OUTPUT);
	pinMode(_motor1_DIR, OUTPUT);
	pinMode(_motor2_PWM, OUTPUT);
	pinMode(_motor2_DIR, OUTPUT);
	digitalWrite(_motor1_PWM, LOW);
	digitalWrite(_motor1_DIR, LOW);
	digitalWrite(_motor2_PWM, LOW);
	digitalWrite(_motor2_DIR, LOW);

}
void Motors::setSpeedMotor1(int speed){
    int motor_dir = MOTOR_1_1;
    int motor_speed = MOTOR_1_0;
    
    if (speed > 255) speed = 255;
    if (speed < -255) speed = -255;
    
    if (speed > 0) {
        if (currentSpeed1 < 0) {    // if speed abruptly changes rest 1 ms
            digitalWrite(motor_dir, LOW);
            analogWrite(motor_speed, 0);
            delayMicroseconds(1000);
        }
        digitalWrite(motor_dir, LOW);
        analogWrite(motor_speed, speed);
    } else {
        if (currentSpeed1 > 0) {    // if speed abruptly changes rest 1 ms
            digitalWrite(motor_dir, LOW);
            analogWrite(motor_speed, 0);
            delayMicroseconds(1000);
        }
        digitalWrite(motor_dir, HIGH);
        analogWrite(motor_speed, 255+speed);
    }
    currentSpeed1 = speed;
}

void Motors::setSpeedMotor2(int speed){
    int motor_dir = MOTOR_2_1;
    int motor_speed = MOTOR_2_0;
    
    if (speed > 255) speed = 255;
    if (speed < -255) speed = -255;
    
    if (speed > 0) {
        if (currentSpeed2 < 0) {    // if speed abruptly changes rest 1 ms
            digitalWrite(motor_dir, LOW);
            analogWrite(motor_speed, 0);
            delayMicroseconds(1000);
        }
        digitalWrite(motor_dir, LOW);
        analogWrite(motor_speed, speed);
    } else {
        if (currentSpeed2 > 0) {    // if speed abruptly changes rest 1 ms
            digitalWrite(motor_dir, LOW);
            analogWrite(motor_speed, 0);
            delayMicroseconds(1000);
        }
        digitalWrite(motor_dir, HIGH);
        analogWrite(motor_speed, 255+speed);
    }
    currentSpeed2 = speed;
}

void Motors::stopMotors(){
    setSpeedMotor1(0);
    setSpeedMotor2(0);
}

// SensorPanel class

SensorPanel::SensorPanel(void) {
    Wire.begin();        // join i2c bus (address optional for master)
    Serial.begin(9600);  // start serial for
    init();
}

void SensorPanel::init() {
    writeMAX7320(B00000000);   // starting with lights out LD1 & LD2  off
    writeMAX11604(B10000010);
}

unsigned char SensorPanel::readMAX7320() {
    unsigned int current_status;
    Wire.requestFrom(B1011000,1);  // ask 1 byte at the address
    delayMicroseconds(200);
    Serial.print(Wire.available());
    if (1<= Wire.available())  // if there's a byte ready...
        current_status = Wire.read(); // ...read current_status
    return (unsigned char)current_status;
}

void SensorPanel::writeMAX7320(unsigned char dgrp) {
    // set I/O on channel of MAX7320
    Wire.beginTransmission(B1011000); // transmit to device #4
    Wire.write(dgrp);              // sends one byte
    Wire.endTransmission();
}

void SensorPanel::writeMAX11604(unsigned char channel) {
    // setup of MAX11604
    // setup of MAX11604
    Wire.beginTransmission(B1100101);
    Wire.write(B01100001|(channel*2));
    Wire.endTransmission();
    delayMicroseconds(200);
}

unsigned char SensorPanel::readMAX11604() {
    unsigned int current_status;
    Wire.requestFrom(B1100101,1);  // ask 1 byte at the address
    Serial.print(Wire.available());
    if (1<= Wire.available())  // if there's a byte ready...
        current_status = Wire.read(); // ...read current_status
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