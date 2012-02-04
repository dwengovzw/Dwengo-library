// vector remapping

// Bootloader occupy addresses 0x00-0x1FFF and thus cannot be used.
// Thus adresses 0x00, 0x08 and 0x18 (used for reset, high prior interrupt,
// and low prior interrupt) have to be remapped.
extern void _startup(void);
#pragma code REMAPPED_RESET_VECTOR = 0x2000
void _reset(void) {
    _asm goto _startup _endasm
}

#pragma code _HIGH_INTERRUPT_VECTOR = 0x002008
void _high_ISR (void)
{
    ;
}

#pragma code _LOW_INTERRUPT_VECTOR = 0x002018
void _low_ISR (void)
{
    ;
}

#pragma code




#include <dwengoConfig.h>
#include <dwengoBoard.h>
#include <dwengoServo.h>
#include <dwengoMotor.h>
#include <dwengoADC.h>
#include <dwengoUsart.h>

void main(void) {
  unsigned char i;
  char c;

  initBoard();
  initLCD();
  initADC();
  initServo();
  initMotor();
  initUsart();

  clearLCD();
  appendStringToLCD("Welcome 2 Dwengo");
  appendStringToLCD(" www.dwengo.org ");
  backlightOn();


  LEDS = 0b00000001;
  for (i=0; i<7; i++) {
    LEDS <<= 1;  // Rotate to the right
    delay_ms(50);
  }
  for (i=0; i<7; i++) {
    LEDS >>= 1;  // Rotate to the left
    delay_ms(50);
  }
  for (i=0; i<7; i++) {
    LEDS <<= 1;  // Rotate to the right
	LED0 = 1;
    delay_ms(50);
  }
  delay_ms(200);
  LEDS = 0;


  c=0;
  printStringToUSART("Welcome 2 Dwengo!\n\r");
  while (c!='!'){
    while(!DataRdyUSART());
      c = ReadUSART();
  };


  setSpeedMotor1(1023);
  delay_ms(500);
  setSpeedMotor1(-1023);
  delay_ms(500);
  setSpeedMotor1(0);

  setSpeedMotor2(1023);
  delay_ms(500);
  setSpeedMotor2(-1023);
  delay_ms(500);
  setSpeedMotor2(0);

  setPosition(1,0);
  setPosition(2,0);
  delay_ms(500);
  setPosition(1,255);
  setPosition(2,255);
  delay_ms(500);
  setPosition(1,0);
  setPosition(2,0);

  LEDS = 0x00;
  while (SW_C) {}
  while (SW_N) {}
  while (SW_E) {}
  while (SW_S) {}
  while (SW_W) {}
  LEDS = 0xFF;

  while(TRUE){

  }
}