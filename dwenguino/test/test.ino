#include <LiquidCrystal.h>
#include <Servo.h>
#include "pitches.h"

// notes in the melody:
int melody[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

//int timer = 100;
//int pinCount = 8;

// LCD
LiquidCrystal lcd(LCD_RS, LCD_RW, LCD_E, LCD_D0, LCD_D1, LCD_D2, LCD_D3, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
// Servo's
Servo servo1;
Servo servo2;

void setup() {
  // Initialise Dwenguino pins
  initDwengo();
  
  // Setup servo's
  servo1.attach(SERVO_1);
  servo2.attach(SERVO_2);
    
  // Setup serial connection
  Serial.begin(9600);
  Serial1.begin(9600);
  
  // Set text to the LCD
  digitalWrite(LCD_BL, HIGH); // LCD backlight on
  lcd.begin(16, 2);
  lcd.print("Hello");
  lcd.setCursor(0, 1);
  lcd.print("I am a Dwenguino");
  LEDS = 0;
  
/*  // Test buzzer
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
  }*/
  
  // Test DC motors
  digitalWrite(MOTOR_1_1 , LOW);
  analogWrite(MOTOR_1_0 , 255);
  digitalWrite(MOTOR_2_1 , LOW);
  analogWrite(MOTOR_2_0 , 255);
  delay(500);
  // invert motors
  digitalWrite(MOTOR_1_1, HIGH );
  analogWrite(MOTOR_1_0 , 0);
  digitalWrite(MOTOR_2_1 , HIGH );
  analogWrite(MOTOR_2_0 , 0);
  delay(500);
  // stop motors
  digitalWrite(MOTOR_1_1 , LOW );
  analogWrite(MOTOR_1_0 , 0);
  digitalWrite(MOTOR_2_1 , LOW );
  analogWrite(MOTOR_2_0 , 0);
  
  // test Servo motors
  servo1.write(0); // 0 degrees
  servo2.write(180); // 180 degrees
  delay(500);
  servo1.write(180); // 180 degrees
  servo2.write(0); // 0 degrees
  delay(500);
}

void loop() {
  if(digitalRead(SW_N) == 0){
    digitalWrite(LED0, HIGH);
  }
  else if(digitalRead(SW_N) == 1){
    digitalWrite(LED0, LOW);
  }
  
  if(digitalRead(SW_C) == 0){
    digitalWrite(LED1, HIGH);
  }
  else if(digitalRead(SW_C) == 1){
    digitalWrite(LED1, LOW);
  }
  
  if(digitalRead(SW_E) == 0){
    digitalWrite(LED2, HIGH);
  }
  else if(digitalRead(SW_E) == 1){
    digitalWrite(LED2, LOW);
  }
  
  if(digitalRead(SW_S) == 0){
    digitalWrite(LED3, HIGH);
  }
  else if(digitalRead(SW_S) == 1){
    digitalWrite(LED3, LOW);
  }
  
  if(digitalRead(SW_W) == 0){
    digitalWrite(LED4, HIGH);
  }
  else if(digitalRead(SW_W) == 1){
    digitalWrite(LED4, LOW);
  }
  
  digitalWrite(LED5, HIGH);
  digitalWrite(LED6, HIGH);
  digitalWrite(LED7, HIGH);
  Serial.println("TEST Serial USB 9600 OK");
  Serial1.println("TEST Serial1 9600 (bluetooth) OK");
  delay(50);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
  delay(50);
}
