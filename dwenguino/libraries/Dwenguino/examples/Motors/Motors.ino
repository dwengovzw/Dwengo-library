#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

Motors motors(MOTOR_1_0, MOTOR_1_1, MOTOR_2_0, MOTOR_2_1);
int sweep = 0;

void setup() {
  // put your setup code here, to run once:
  initDwenguino();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(sweep=0;sweep<255;sweep++) {
    motors.setSpeedMotor1(sweep);
    motors.setSpeedMotor2(-sweep);
    delay(10);
  }
  motors.stopMotors();
  delay(1000);
  
  for(sweep=0;sweep<255;sweep++) {
    motors.setSpeedMotor1(-sweep);
    motors.setSpeedMotor2(sweep);
    delay(10);
  }  
}
