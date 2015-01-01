#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>
/*
  Read buttons
 */

// the setup function runs once when you press reset or power the board
void setup() {
  initDwenguino();  // initialize all dwenguino functionality
}

// the loop function runs over and over again forever
void loop() {
  
  dwenguinoLCD.clear();
  dwenguinoLCD.print("Buttons = ");
  if (digitalRead(SW_W) == LOW) dwenguinoLCD.print("West ");
  if (digitalRead(SW_S) == LOW) dwenguinoLCD.print("South ");
  if (digitalRead(SW_E) == LOW) dwenguinoLCD.print("East ");
  if (digitalRead(SW_N) == LOW) dwenguinoLCD.print("North ");
  if (digitalRead(SW_C) == LOW) dwenguinoLCD.print("Center ");

  delay(500);

}
  
