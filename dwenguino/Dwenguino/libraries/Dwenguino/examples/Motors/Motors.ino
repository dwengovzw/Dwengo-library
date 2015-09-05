#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

#include <DwenguinoMotor.h>
#include <Servo.h>

// construct a Servo and a DCMotor object
Servo servo1;
DCMotor motor1(MOTOR_1_0, MOTOR_1_1);

int pos = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  initDwenguino();
  servo1.attach(SERVO_1);
}

// the loop function runs over and over again forever
void loop() {

  motor1.setSpeed(255);                 // set speed of DC motor to rotate clockwise at full speed 255

  for(pos = 0; pos <= 180; pos += 3) {  // goes from 0 degrees to 180 degrees in steps of 1 degree 
    servo1.write(pos);                  // tell servo to go to position in variable 'pos' 
    delay(15);                          // wait some time
  } 

  motor1.setSpeed(-255);                // set speed of DC motor to rotate counterclockwise at full speed -255
  
  for(pos = 180; pos>=0; pos-=3) {      // goes from 180 degrees to 0 degrees                          
    servo1.write(pos);                  // tell servo to go to position in variable 'pos'  
    delay(15);         
 } 

  motor1.setSpeed(100);                 // set speed of DC motor to rotate clockwise at intermediate speed
  delay(2000);

}
