#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>
#include <DwenguinoMotor.h> // the motor library

Motor myservo(SERVO_1); // create servo object to control a servo attached to SERVO_1
 
int potpin = 0;  // analog pin used to connect the potentiometer
int val;               // variable to read the value from the analog pin 
 
void setup() 
{ 
  initDwenguino();
  myservo.init();  // attaches the servo on pin 9 to the servo object 
  
} 
 
void loop() { 
  val = analogRead(potpin);          // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179); // scale it to use it with the servo (value between 0 and 180) 
  myservo.setCommand(val);       // sets the servo position according to the scaled value 
  delay(10);                                   // waits for the servo to get there 
}
