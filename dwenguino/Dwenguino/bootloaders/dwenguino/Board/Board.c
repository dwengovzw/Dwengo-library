/* Dwenguino Bootloader LCD fuctions
*   Modified on Dec 23 2017 by Jelle Roets from Dwengo vzw (www.dwengo.org)
*/

#include "Board.h"

static void LCD_sendByte(bool command, uint8_t value){    
    if(command) LCD_RS_Low(); // set RS pin to command (low) and data (high) mode
    else LCD_RS_High();

    LCD_setDataPort(value);  // write data byte
    LCD_E_High();           // Pulse E pin
    _delay_us(1);
    LCD_E_Low();           

    _delay_us(50);          // normally commands take about 37 us to settle
}

void LCD_clear(){
    LCD_sendByte(true, LCD_CLEARDISPLAY);
    _delay_us(1700);
}

void LCD_init(){
    LCD_PortInit();
    LCD_RW_Low();
    _delay_ms(15);  // according to Datasheet: wait 40 ms after powerup: but power is normally already on before bootloader is started! TODO: necessary??
    LCD_sendByte(true, LCD_FUNCTIONSET | LCD_8BITMODE | LCD_2LINE | LCD_5x8DOTS); // TODO: repeat?
    LCD_sendByte(true, LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
    LCD_sendByte(true, LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
    LCD_clear();
}

void LCD_print(const char* str){
    while(*str) LCD_sendByte(false, *str++);
}

void LCD_setCursor(uint8_t row, uint8_t col){
    LCD_sendByte(true, LCD_SETDDRAMADDR | ((row == 0 ? 0x00 : 0x40) + col));
}
