#include <LiquidCrystal.h>
#include <Wire.h>
#include <Dwenguino.h>

#include <Pitches.h>

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize all the dwenguino functions
  initDwenguino();
}

// the loop function runs over and over again forever
void loop() {
  int count = 0;  // define some variables here

  // Print something on the LCD screen:
  dwenguinoLCD.clear();
  dwenguinoLCD.print("Play some melody");
  
  int melody[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4}; // notes in the melody:
  int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4}; // note durations: 4 = quarter note, 8 = eighth note, etc.:

  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(BUZZER, melody[thisNote],noteDuration);
    
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER);
  }
}
