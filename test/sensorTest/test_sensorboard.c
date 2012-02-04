#include "dwengoBoard.h"
#include "dwengoConfig.h"
#include "dwengoDelay.h"
#include "dwengoSensorModule.h"

#define THRESHOLD	140	// difference between amient and active measured distance to objects

void main() {
  initBoard();
  initSensorModule();
  clearLCD();
  appendStringToLCD("Test: ");
  appendStringToLCD("OS1");
  while((int)readSensor(OS1,ACTIVE_MODE)>THRESHOLD);
  clearLCD();
  appendStringToLCD("Test: ");
  appendStringToLCD("OS2");
  while((int)readSensor(OS2,ACTIVE_MODE)>THRESHOLD);
  clearLCD();
  appendStringToLCD("Test: ");
  appendStringToLCD("OS3");
  while((int)readSensor(OS3,ACTIVE_MODE)>THRESHOLD);
  clearLCD();
  appendStringToLCD("Test: ");
  appendStringToLCD("OS4");
  while((int)readSensor(OS4,ACTIVE_MODE)>THRESHOLD);
  clearLCD();
  appendStringToLCD("Test: ");
  appendStringToLCD("OS5");
  while((int)readSensor(OS5,ACTIVE_MODE)>THRESHOLD);
  clearLCD();
  appendStringToLCD("Test: ");
  appendStringToLCD("OS6");
  while((int)readSensor(OS6,ACTIVE_MODE)>THRESHOLD);
  clearLCD();
  appendStringToLCD("Test: ");
  appendStringToLCD("OS7");
  while((int)readSensor(OS7,ACTIVE_MODE)>THRESHOLD);
  clearLCD();
  appendStringToLCD("Test: ");
  appendStringToLCD("D1");
  while(((int)readSensor(D1,ACTIVE_MODE))<253); // if dark
  clearLCD();
  appendStringToLCD("Test: ");
  appendStringToLCD("D2");
  while(((int)readSensor(D2,ACTIVE_MODE))<253); // if dark
  setHeadlights(LD1,TRUE);
  setHeadlights(LD2,TRUE);
  clearLCD();
  appendStringToLCD("Test: ");
  appendStringToLCD("OS8");
  powerLongRange(TRUE);
  while(TRUE) {
	LEDS = readSensor(OS8,ACTIVE_MODE);
	delay_ms(50);
  }
}