/*
 * Motor driver
 * part of Dwengo library
 *
 * Control of motor driver on the Dwengo board
 *
 * Version: 1.0.$Revision: 2121 $
 * Date: $Date: 2010-05-01 23:09:41 +0200 (Sat, 01 May 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

// wait time to minimize transients in the motors when introducing abrupt speed changes, in ms
#define TRANSIENT_DELAY 20

#include "compiler/pwm.h"

#include "dwengoBoard.h"
#include "dwengoMotor.h"
#include "dwengoDelay.h"


void initMotor() {
  // Set the motor outputs as digital output
  MOTOR1_0_DIR = 0;
  MOTOR1_1_DIR = 0;
  MOTOR2_0_DIR = 0;
  MOTOR2_1_DIR = 0;

  // Maximal PWM freq: 5000Hz (using L293DNE motor driver)
  // PWM period = (0xFF + 1)*4*Tosc*T2Prescaler
  OpenPWM1(0xFF);   // configuring PWM module 1
  OpenPWM2(0xFF);   // configuring PWM module 2

  // Configuring timer 2 which provides timing for PWM
  // TIMER_INT_OFF: disable timer interrupt
  // T2_PS_1_4: Timer2 prescaling set to 4
  // T2_POST_1_1: Timer2 postscaling set to 1
  OpenTimer2(TIMER_INT_OFF & T2_PS_1_4 & T2_POST_1_1);

  setSpeedMotor1(0);
  setSpeedMotor2(0);
}

void setSpeedMotor1(short speed) {
  static BYTE prev_dir;
  BYTE dir;
  dir = (speed > 0);

  // check if motor has changed direction
  if (dir != prev_dir) {
    // stop motor
    SetDCPWM1(0);
    // wait a short time
    delay_ms(TRANSIENT_DELAY);
    // update direction
    prev_dir = dir;
  }

  if (dir == FORWARD) {
    SetDCPWM1(speed);
    MOTOR1_1 = 0;
  }
  else { // speed is negative
    SetDCPWM1(1023+speed);
    MOTOR1_1 = 1;
  }
}

void setSpeedMotor2(short speed) {
  static BYTE prev_dir;
  BYTE dir;
  dir = (speed > 0);

  // check if motor has changed direction
  if (dir != prev_dir) {
    // stop motor
    SetDCPWM2(0);
    // wait a short time
    delay_ms(TRANSIENT_DELAY);
    // update direction
    prev_dir = dir;
  }

  if (dir == FORWARD) {
    SetDCPWM2(speed);
    MOTOR2_1 = 0;
  }
  else { // speed is negative
    SetDCPWM2(1023+speed);
    MOTOR2_1 = 1;
  }
}

void stopMotors() {
  SetDCPWM1(0);
  MOTOR1_1 = 0;

  SetDCPWM2(0);
  MOTOR2_1 = 0;
}
