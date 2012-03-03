/*
 * LCD Display
 * part of Dwengo library
 *
 * Control of LCD display on the Dwengo board
 *
 * Version: 1.0.$Revision: 4548 $
 * Date: $Date: 2011-10-09 16:18:41 +0200 (Sun, 09 Oct 2011) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

#include "dwengoLCD.h"
#include <stdlib.h>

struct lcd_info_type {
  unsigned char line;
  unsigned char pos;
};

// Keeps track of current line number and character position.
struct lcd_info_type lcd_info;

void initLCD(void) {
	LCD_BACKLIGHT_DIR = OUTPUT;   // Set LCD backlight-pin as output
	ADCON1 = 0x0A;                // RE0-2 digital
	delay_ms(5);                  // wait 5ms

	LCD_DATA = 0;
	LCD_DATA_DIR = 0xff;

	LCD_RS_DIR = OUTPUT;
	LCD_RW_DIR = OUTPUT;
	LCD_EN_DIR = OUTPUT;

	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;

	delay_ms(15);

	LCD_DATA_DIR = 0;

	commandLCD(0b00110000);
	commandLCD(0b00110000);
	commandLCD(0b00110000);
	commandLCD(0b00111000);
	commandLCD(0b00000110);
	commandLCD(0b00011000);
	commandLCD(0b00001100);
	lcd_info.line = lcd_info.pos = 0;
	clearLCD();
}

void clearLCD(void) {
	commandLCD(0b00000001);
	delay_ms(2);
	setCursorLCD(0,0);
}

void commandLCD(const BYTE c) {
	BYTE temp;

	temp = LCD_DATA;	 // Save current data on datapins for LED's

	// Write command to LCD
	LCD_RW = 0;
	LCD_RS = 0;

	LCD_EN = 1;       // Bring enable pin high
	temp = LCD_DATA;      // Save current data on datapins for LED's
	LCD_DATA   = c;       // Write command data to data port
	LCD_EN = 0;       // data is clocked at the falling edge of enable pin
	LCD_DATA   = temp;    // restore data on datapins
	delay_us(45);         // wait until display has processed the data
}


// set cursor at position p of line l
void setCursorLCD(BYTE l, BYTE p) {
	BYTE c;

	// check if input is valid
	if (l>LCD_LASTLINE) l = LCD_LASTLINE;
	if (p>LCD_LASTPOS)  p = LCD_LASTPOS;

	lcd_info.line = l;
	lcd_info.pos = p;

	if (l == 1)
		c = 0b11000000;
	else
		c = 0b10000000;

	// position offset
	p &= 0b00001111;
	c |= p;

	commandLCD(c);

	delay_us(45);         // wait until display has processed the command
}


void appendCharToLCD(const char c) {
  unsigned temp = LCD_DATA;   // Save current data on datapins for LED's
  if (lcd_info.pos>LCD_LASTPOS)
    if (lcd_info.line)
      clearLCD();
    else
      setCursorLCD(1, 0);

	// Write char to LCD
	LCD_RW = 0;
	LCD_RS = 1;

	LCD_EN     = 1;       // Bring enable pin high
	temp = LCD_DATA;      // Save current data on datapins for LED's
	LCD_DATA   = c;       // Write data to data port
	LCD_EN     = 0;       // data is clocked at the falling edge of enable pin
	LCD_DATA   = temp;    // restore data on datapins
	delay_us(45);         // wait until display has processed the data

	lcd_info.pos++;       // increment lcd cursor position
}


void printCharToLCD(const char c, BYTE l, BYTE p) {
	BYTE temp;

	// set cursor to selected position
	setCursorLCD(l,p);

	// append to this position
	appendCharToLCD(c);
}

void appendStringToLCD_(const FAR_ROM char* message) {
	while (*message) {
		appendCharToLCD(*message++);
	}
}

void printStringToLCD(char* message, BYTE l, BYTE p) {
	// set cursor to selected position
	setCursorLCD(l,p);

	// append to this position
	appendStringToLCD(message);
}

void appendStringToLCDcharptr(char* message) {
  while(*message) {
    appendCharToLCD(*message++);
  }
}

void appendIntToLCD(int i) {
  char buffer[7];
#ifdef SDCC_pic16
  //sdcc needs the radix as 3rd argument
  itoa(i,buffer,10);
#else
  itoa(i,buffer);
  buffer[6] = 0;
#endif
  appendStringToLCDcharptr(buffer);
}

void printIntToLCD(int i, BYTE l, BYTE p) {
	// set cursor to selected position
	setCursorLCD(l,p);

	// append to this position
	appendIntToLCD(i);
}

