#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

void setup() {
  // put your setup code here, to run once:
  initDwenguino();
  dwenguinoLCD.print("hello");
}

void loop() {
  // put your main code here, to run repeatedly:

}
