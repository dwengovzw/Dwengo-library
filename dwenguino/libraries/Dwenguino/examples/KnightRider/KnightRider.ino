#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

void setup() {
  initDwenguino();
  LEDS = 0b00000001;
}
void loop() {
  for (unsigned char i=0; i<7; i++) {
    LEDS <<= 1;  // Rotate to the right
    delay(50);
  }
  for (unsigned char i=0; i<7; i++) {
    LEDS >>= 1;  // Rotate to the left
    delay(50);
  }
}

