/*
  Read out analog pin
  
 */

#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

int analogPin = A0;


// the setup function runs once when you press reset or power the board
void setup() {
  initDwenguino();
}

// the loop function runs over and over again forever
void loop() {
  int val;
  val = analogRead(analogPin);
  
  dwenguinoLCD.clear();
  dwenguinoLCD.print("Value = ");
  dwenguinoLCD.print(1023-val);

  delay(100);
}
  
