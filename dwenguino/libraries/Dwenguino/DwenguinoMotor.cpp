/* ---------------------------------------------------------------------------
	Dwenguino Library - v2.0 
	
	Created on Jan 09 2015 by Naveen Kuppuswamy from Dwengo vzw (www.dwengo.org)

Motor.cpp - Library combining DC Motor control  and a Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.

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
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <avr/interrupt.h>
#include <Arduino.h>

#include "DwenguinoMotor.h"

static servo_t servos[MAX_SERVOS];                          // static array of servo structures
static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

uint8_t ServoCount = 0;                                     // the total number of attached servos


/************ static functions common to all instances (for servo library) ***********************/

static inline void handle_interrupts(timer16_Sequence_t timer, volatile uint16_t *TCNTn, volatile uint16_t* OCRnA)
{
  if( Channel[timer] < 0 )
    *TCNTn = 0; // channel set to -1 indicated that refresh interval completed so reset the timer
  else{
    if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && SERVO(timer,Channel[timer]).Pin.isActive == true )
      digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,LOW); // pulse this channel low if activated
  }

  Channel[timer]++;    // increment to the next channel
  if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER) {
    *OCRnA = *TCNTn + SERVO(timer,Channel[timer]).ticks;
    if(SERVO(timer,Channel[timer]).Pin.isActive == true)     // check if activated
      digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,HIGH); // its an active channel so pulse it high
  }
  else {
    // finished all channels so wait for the refresh period to expire before starting over
    if( ((unsigned)*TCNTn) + 4 < usToTicks(REFRESH_INTERVAL) )  // allow a few ticks to ensure the next OCR1A not missed
      *OCRnA = (unsigned int)usToTicks(REFRESH_INTERVAL);
    else
      *OCRnA = *TCNTn + 4;  // at least REFRESH_INTERVAL has elapsed
    Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
  }
}


// Interrupt handlers for Arduino
SIGNAL (TIMER1_COMPA_vect)
{
  handle_interrupts(_timer1, &TCNT1, &OCR1A);
}


static void initISR(timer16_Sequence_t timer)
{
   if(timer == _timer1) {
    TCCR1A = 0;             // normal counting mode
    TCCR1B = _BV(CS11);     // set prescaler of 8
    TCNT1 = 0;              // clear the timer count
    // here if not ATmega8 or ATmega128
    TIFR1 |= _BV(OCF1A);     // clear any pending interrupts;
    TIMSK1 |=  _BV(OCIE1A) ; // enable the output compare interrupt
  }
}

static void finISR(timer16_Sequence_t timer)
{
    //disable use of the given timer
    //For arduino - in future: call here to a currently undefined function to reset the timer
}

static boolean isTimerActive(timer16_Sequence_t timer)
{
  // returns true if any servo is active on this timer
  for(uint8_t channel=0; channel < SERVOS_PER_TIMER; channel++) {
    if(SERVO(timer,channel).Pin.isActive == true)
      return true;
  }
  return false;
}

/****************** end of static functions (for servo library) ******************************/


/****************** Servo Library ******************************/
Servo::Servo()
{
  if( ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
	servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values  - 12 Aug 2009
  }
  else
    this->servoIndex = INVALID_SERVO ;  // too many servos
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  if(this->servoIndex < MAX_SERVOS ) {
    pinMode( pin, OUTPUT) ;                                   // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
    this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max)/4;
    // initialize the timer if it has not already been initialized
    timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if(isTimerActive(timer) == false)
      initISR(timer);
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
  return this->servoIndex ;
}

void Servo::detach()
{
  servos[this->servoIndex].Pin.isActive = false;
  timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
  if(isTimerActive(timer) == false) {
    finISR(timer);
  }
}

void Servo::write(int value)
{
  if(value < MIN_PULSE_WIDTH)
  {  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    if(value < 0) value = 0;
    if(value > 180) value = 180;
    value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());
  }
  this->writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    if( value < SERVO_MIN() )          // ensure pulse width is valid
      value = SERVO_MIN();
    else if( value > SERVO_MAX() )
      value = SERVO_MAX();

    value = value - TRIM_DURATION;
    value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead - 12 Aug 2009

    uint8_t oldSREG = SREG;
    cli();
    servos[channel].ticks = value;
    SREG = oldSREG;
  }
}

int Servo::read() // return the value as degrees
{
  return  map( this->readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if( this->servoIndex != INVALID_SERVO )
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks)  + TRIM_DURATION ;   // 12 aug 2009
  else
    pulsewidth  = 0;

  return pulsewidth;
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive ;
}


/****************** DC Motor Library ******************************/

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
    int motor_dir = _motor_PWM;
    int motor_speed = _motor_DIR;
    
    if (speed > 255) speed = 255;
    if (speed < -255) speed = -255;
    
    if (speed > 0) {
        if (currentSpeed < 0) {    // if speed abruptly changes rest 1 ms
            digitalWrite(motor_dir, LOW);
            analogWrite(motor_speed, 0);
            delayMicroseconds(1000);
        }
        digitalWrite(motor_dir, LOW);
        analogWrite(motor_speed, speed);
    } else {
        if (currentSpeed > 0) {    // if speed abruptly changes rest 1 ms
            digitalWrite(motor_dir, LOW);
            analogWrite(motor_speed, 0);
            delayMicroseconds(1000);
        }
        digitalWrite(motor_dir, HIGH);
        analogWrite(motor_speed, 255+speed);
    }
    currentSpeed = speed;
}

/****************** Motor Library ******************************/

Motor::Motor(uint8_t servo_pin)
{
	type = Servo_Motor;
	servo = new Servo;
	dcMotor = NULL;
	servoPin = servo_pin;
}
Motor::Motor(uint8_t motor_PWM_pin, uint8_t motor_DIR_pin)
{
	type = DC_Motor;
	dcMotor = new DCMotor(motor_PWM_pin,motor_DIR_pin);
	servo = NULL;
}
void Motor::init()
{

	if(type == Servo_Motor)
	{
		servo->attach(servoPin);
	}	

}
void Motor::setCommand(int command)
{
	if(type == DC_Motor)
	{	
		dcMotor->setSpeed(command);
	}
	else
	{
		servo->write(command);
	}
}
Motor::~Motor()
{
	if(servo!=NULL)
		delete(servo);
	else if(dcMotor!=NULL)
		delete(dcMotor);
}
	
