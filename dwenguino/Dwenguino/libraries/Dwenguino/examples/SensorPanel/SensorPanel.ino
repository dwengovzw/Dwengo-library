#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

#include <DwenguinoSensorPanel.h>

SensorPanel sensorpanel;

// Pin 14 and 15 (I2C) are reserved for communication to the IOBoard

void setup() {
  // put your setup code here, to run once:
  initDwenguino();
  sensorpanel = SensorPanel();
  sensorpanel.init();
  sensorpanel.powerLongRange(true); // if you want to use the long range distance sensor, you need to activate it
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char sensor;
  sensorpanel.setHeadlights(LD2,true);
  delay(200);
  sensorpanel.setHeadlights(LD2,false);
  delay(200);
  
  dwenguinoLCD.clear();
  dwenguinoLCD.setCursor(0, 0);
  sensor = sensorpanel.readSensor(OS1,DIFF_MODE);
  dwenguinoLCD.print("Sensor OS1: ");
  dwenguinoLCD.print((int)sensor);
  dwenguinoLCD.setCursor(0, 1);
  sensor = sensorpanel.readSensor(OS8,ACTIVE_MODE);
  dwenguinoLCD.print("Sensor OS8: ");
  dwenguinoLCD.print((int)sensor);
}
