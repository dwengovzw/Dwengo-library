/*
  Control the dwenguino board
 */
 
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Pitches.h>
#include <Dwenguino.h>


#define PRESSED 0

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

LiquidCrystal lcd(LCD_RS, LCD_RW, LCD_E, LCD_D0, LCD_D1, LCD_D2, LCD_D3, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize all the dwenguino functions
  initDwenguino();
  
  // wait until you press the start button
  lcd.print("Press C to start");
  while (digitalRead(SW_C) != PRESSED);
}

// the loop function runs over and over again forever
void loop() {
  int count = 0;  // define some variables here

  // Print something on the LCD screen:
  lcd.clear();
  lcd.print("Play some melody");
  
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

  while (digitalRead(SW_E) != PRESSED); // wait until you press the east button
  while (digitalRead(SW_E) == PRESSED); // wait until you release the east button to go to the next program

  
}
  
  
void initDwenguino(){
  initDwengo();
  
  // initialize LCD
  digitalWrite(LCD_BL, HIGH);
  lcd.begin(16,2);
  
  // initialize Buttons
  pinMode(SW_W, INPUT_PULLUP);
  pinMode(SW_S, INPUT_PULLUP);
  pinMode(SW_E, INPUT_PULLUP);
  pinMode(SW_N, INPUT_PULLUP);
  pinMode(SW_C, INPUT_PULLUP);
  
  
  pinMode(MOTOR_1_0, OUTPUT);
  pinMode(MOTOR_1_1, OUTPUT);
  pinMode(MOTOR_2_0, OUTPUT);
  pinMode(MOTOR_2_1, OUTPUT); 
  
  }
  
void setSpeedMotor(int motor, int speed) {  // This function sets the speed of a specific motor: 
  int motor_dir = (motor == 2) ? MOTOR_2_1 : MOTOR_1_1;
  int motor_speed = (motor == 2) ? MOTOR_2_0 : MOTOR_1_0;
  
  if (speed > 255) speed = 255;
  if (speed < -255) speed = -255;

  if (speed > 0) {
    digitalWrite(motor_dir, LOW);
    analogWrite(motor_speed, speed);
  } else {
    digitalWrite(motor_dir, HIGH);
    analogWrite(motor_speed, 255+speed);
  }
}

void stopMotors() {
  setSpeedMotor(1,0);
  setSpeedMotor(2,0);
}

int readUltrasonic(){
    unsigned int uS = sonar.ping();
    return uS / US_ROUNDTRIP_CM;
}
