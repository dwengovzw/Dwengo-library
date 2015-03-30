// Dwenguino Sensors example
// includes: Sonar , Temperature and humidity, Sound impact, Color

// standard dwenguino libraries
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

// sensor libraries
#include <NewPing.h> // necessary for sonar: http://playground.arduino.cc/Code/NewPing
#include <dht.h> // necessary for temperature sensor: https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib and http://playground.arduino.cc/Main/DHTLib

// macro's
#define TRIGGER_PIN  12  // Dwenguino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Dwenguino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define SOUND_PIN 10 // Dwenguino pin tied to sound impact sensor output

#define DHT11PIN 3  // Dwenguino pin tied to the DHT11 signal pin

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // make a sonar object

dht DHT; // make an DHT object

void setup() {
  // put your setup code here, to run once:
  initDwenguino();
  pinMode(SOUND_PIN, INPUT); // set the sound impact sensor input pin
}

void loop() {
  // put your main code here, to run repeatedly:

  // Sonar
  if(digitalRead(SW_N) == LOW) {
    unsigned int distance = sonar.ping_cm();  // read sonar value in cm
    dwenguinoLCD.clear();
    dwenguinoLCD.print("Sonar = ");
    dwenguinoLCD.print(distance);
    dwenguinoLCD.print(" cm");
  }

  // Temperature sensor (DHT11)
  if(digitalRead(SW_C) == LOW) {
    int chk = DHT.read11(DHT11PIN);
    dwenguinoLCD.clear();
    dwenguinoLCD.print("Temp = ");
    dwenguinoLCD.print(DHT.temperature);
    dwenguinoLCD.print(" C");
  }
  
  // sound impact
  if(digitalRead(SW_S) == LOW) {
    LEDS = 0;
    unsigned char sound = 0;
    for(int i=0;i<255;i++) {
      if(digitalRead(SOUND_PIN)==LOW)
        sound++;
      delay(1);
    }
    LEDS = sound;
    dwenguinoLCD.clear();
    dwenguinoLCD.print("Sound = ");
    dwenguinoLCD.print(sound);
  }
  delay(100);
}
