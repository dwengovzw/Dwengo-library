#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

#include <DwenguinoIOBoard.h>

// Please note that JP1-1 and JP1-2 must be set
// Pin 14 and 15 (I2C) are reserved for communication to the IOBoard

IOBoard io(0);  // create an IOBoard object with address 0 (see jumper ADDRESS on Dwengo IO Board)

void setup() {
  // put your setup code here, to run once:
  initDwenguino();
  io.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  dwenguinoLCD.clear();
  dwenguinoLCD.print("IO status = ");
  io.setOutputs(B11110000);
  dwenguinoLCD.print(io.readInputs());
  delay(1000);
  io.setOutputs(B00001111);
  delay(1000);
}
