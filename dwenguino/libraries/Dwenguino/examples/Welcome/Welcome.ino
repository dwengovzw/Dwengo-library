#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>
/*
  Read buttons
 */

// the setup function runs once when you press reset or power the board
void setup() {
  initDwenguino();  // initialize all dwenguino functionality
  pinMode(13, OUTPUT);
  
  dwenguinoLCD.clear();
  dwenguinoLCD.print("Welcome to");
  dwenguinoLCD.setCursor(0,1);
  dwenguinoLCD.print("Dwenguino");
  LEDS=0b10101010;
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
  
