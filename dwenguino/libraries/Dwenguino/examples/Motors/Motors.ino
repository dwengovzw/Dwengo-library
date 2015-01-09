#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>
#include <DwenguinoMotor.h>

Motor servo1(SERVO_1);
Motor dcMotor1(MOTOR_1_0, MOTOR_1_1);

int pos = 0;
void setup() {
  initDwenguino();
  servo1.init();

}

void loop() {
  // put your main code here, to run repeatedly:
  for(pos = 0; pos <= 180; pos += 3) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    servo1.setCommand(pos);  // tell servo to go to position in variable 'pos' 
     delay(15); 
    
    dcMotor1.setCommand(225);// set DC motor to rotate clockwise with constant velocity, the maximal speed is 255
  } 
  
  for(pos = 180; pos>=0; pos-=3)     // goes from 180 degrees to 0 degrees 
  {                                
    servo1.setCommand(pos);// tell servo to go to position in variable 'pos'  
    delay(15);         

    dcMotor1.setCommand(-225);// set DC motor to rotate counter clockwise with constant velocity, the minimal speed is -255
 }   
}
