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
  if (digitalRead(SW_W) == PRESSED) dwenguinoLCD.print("West ");
  if (digitalRead(SW_S) == PRESSED) dwenguinoLCD.print("South ");
  if (digitalRead(SW_E) == PRESSED) dwenguinoLCD.print("East ");
  if (digitalRead(SW_N) == PRESSED) dwenguinoLCD.print("North ");
  if (digitalRead(SW_C) == PRESSED) dwenguinoLCD.print("Center ");

  delay(500);

}
  
