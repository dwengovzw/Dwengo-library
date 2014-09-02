/*
  pins_arduino.h - Pin definition functions for Arduino adapted to work with Dwenguino v2.0
  Part of the Dwengo library

  Originally made by David A. Mellis,
  Adapted by Sam De Bodt, Jeller Roets and Francis wyffels from Dwengo vzw in 2014

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS  48
#define NUM_ANALOG_INPUTS 8

// No TX or RX led connected 
//#define TX_RX_LED_INIT	//DDRD |= (1<<5), DDRB |= (1<<0)
//#define TXLED0			//PORTD |= (1<<5)
//#define TXLED1			//PORTD &= ~(1<<5)
//#define RXLED0			//PORTB |= (1<<0)
//#define RXLED1			//PORTB &= ~(1<<0)

static const uint8_t SDA = 15;
static const uint8_t SCL = 14;

// Map SPI port to 'new' pins 
static const uint8_t SS   = 10;
static const uint8_t MOSI = 2;
static const uint8_t MISO = 12;
static const uint8_t SCK  = 13;

// Mapping of analog pins as digital I/O
// A6-A11 share with digital pins
static const uint8_t A0 = 24;
static const uint8_t A1 = 25;
static const uint8_t A2 = 26;
static const uint8_t A3 = 27;
static const uint8_t A4 = 28;
static const uint8_t A5 = 29;
static const uint8_t A6 = 30; 
static const uint8_t A7 = 31;

// Dwengo functionality
// LEDs
#define LEDS_DIR DDRA
#define LEDS PORTA
static const uint8_t LED0 = 32;
static const uint8_t LED1 = 33;
static const uint8_t LED2 = 34;
static const uint8_t LED3 = 35;
static const uint8_t LED4 = 36;
static const uint8_t LED5 = 37;
static const uint8_t LED6 = 38;
static const uint8_t LED7 = 39;

// Buzzer
static const uint8_t BUZZER = 46;

// Buttons
static const uint8_t SW_C = 47;
static const uint8_t SW_N = 16;
static const uint8_t SW_E = 17;
static const uint8_t SW_S = 18;
static const uint8_t SW_W = 19;

// DC motor connector
static const uint8_t MOTOR_1_0 = 44; // Motor PWM
static const uint8_t MOTOR_1_1 = 43; // Motor direction
static const uint8_t MOTOR_2_0 = 45; // Motor PWM
static const uint8_t MOTOR_2_1 = 42; // Motor direction

// Servo motors
static const uint8_t SERVO_1 = 40;
static const uint8_t SERVO_2 = 41;

// Dwengo LCD
static const uint8_t LCD_BL = 20;
static const uint8_t LCD_E = 21;
static const uint8_t LCD_RW = 22;
static const uint8_t LCD_RS = 23;
static const uint8_t LCD_D0 = 32;
static const uint8_t LCD_D1 = 33;
static const uint8_t LCD_D2 = 34;
static const uint8_t LCD_D3 = 35;
static const uint8_t LCD_D4 = 36;
static const uint8_t LCD_D5 = 37;
static const uint8_t LCD_D6 = 38;
static const uint8_t LCD_D7 = 39;

// PCINT 0-7 are on pins 6-13
#define digitalPinToPCICR(p)    ( (((p) >= 6) && ((p) <= 13)) ? (&PCICR) : ((uint8_t *)0) )
#define digitalPinToPCICRbit(p) 0
#define digitalPinToPCMSK(p)    ( (((p) >= 6) && ((p) <= 13)) ? (&PCMSK0) : ((uint8_t *)0) )
#define digitalPinToPCMSKbit(p) ( (((p) >= 6) && ((p) <= 13)) ? ((p) - 6) : 0 )

extern const uint8_t PROGMEM analog_pin_to_channel_PGM[];
#define analogPinToChannel(P)  ( pgm_read_byte( analog_pin_to_channel_PGM + (P) ) )

// Initialise Dwenguino board pins
#define initDwengo() \
{ \
    LEDS_DIR = 0xFF;    \
    DDRC = 0b01111111;  \
    PORTC = 0b10000000; \
    DDRE = 0b00001111;  \
    PORTE = 0b11110000; \
}


#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
  NOT_A_PORT,
  (uint16_t) &DDRA,
  (uint16_t) &DDRB,
  (uint16_t) &DDRC,
  (uint16_t) &DDRD,
  (uint16_t) &DDRE,
  (uint16_t) &DDRF,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
  NOT_A_PORT,
    (uint16_t) &PORTA,
  (uint16_t) &PORTB,
  (uint16_t) &PORTC,
  (uint16_t) &PORTD,
  (uint16_t) &PORTE,
  (uint16_t) &PORTF,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
  NOT_A_PORT,
  (uint16_t) &PINA,
  (uint16_t) &PINB,
  (uint16_t) &PINC,
  (uint16_t) &PIND,
  (uint16_t) &PINE,
  (uint16_t) &PINF,
};


const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
    PD, // PD2 - D0  - RX1
    PD, // PD3 - D1  - TX1

    PB, // PB2 - D2  - MOSI 
    PB, // PB7 - D3  -

    PD, // PD7 - D4  -

    PB, // PB6 - D5  -
    PB, // PB5 - D6  -

    PD, // PD6 - D7  -
    PD, // PD5 - D8  -
    PD, // PD4 - D9  -

    PB, // PB0 - D10 - SS
    PB, // PB4 - D11 -
    PB, // PB3 - D12 - MISO
    PB, // PB1 - D13 - LED13  -  SCK

    PD, // PD0 - D14 - SCL
    PD, // PD1 - D15 - SDA

    PE, // PE7 - D16 - SW_N
    PE, // PE6 - D17 - SW_E
    PE, // PE5 - D18 - SW_S
    PE, // PE4 - D19 - SW_W
    PE, // PE3 - D20 - LCD_BL
    PE, // PE2 - D21 - LCD_E
    PE, // PE1 - D22 - LCD_RW
    PE, // PE0 - D23 - LCD_RS

    PF, // PF7 - D24 - A0
    PF, // PF6 - D25 - A1
    PF, // PF5 - D26 - A2
    PF, // PF4 - D27 - A3
    PF, // PF3 - D28 - A4
    PF, // PF2 - D29 - A5
    PF, // PF1 - D30 - A6
    PF, // PF0 - D31 - A7

    PA, // PA0 - D32 - LED0   - LCD_DB0
    PA, // PA1 - D33 - LED1   - LCD_DB1
    PA, // PA2 - D34 - LED2   - LCD_DB2
    PA, // PA3 - D35 - LED3   - LCD_DB3
    PA, // PA4 - D36 - LED4   - LCD_DB4
    PA, // PA5 - D37 - LED5   - LCD_DB5
    PA, // PA6 - D38 - LED6   - LCD_DB6
    PA, // PA7 - D39 - LED7   - LCD_DB7

    PC, // PC0 - D40 - SERVO_1
    PC, // PC1 - D41 - SERVO_2
    PC, // PC2 - D42 - 4A
    PC, // PC3 - D43 - 2A
    PC, // PC4 - D44 - 3A
    PC, // PC5 - D45 - 1A
    PC, // PC6 - D46 - BUZZER
    PC, // PC7 - D47 - SW_C

};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
    _BV(2), // PD2 - D0  - RX1
    _BV(3), // PD3 - D1  - TX1

    _BV(2), // PB2 - D2  - MOSI
    _BV(7), // PB7 - D3  -

    _BV(7), // PD7 - D4  -

    _BV(6), // PB6 - D5  -
    _BV(5), // PB5 - D6  -

    _BV(6), // PD6 - D7  -
    _BV(5), // PD5 - D8  -
    _BV(4), // PD4 - D9  -

    _BV(0), // PB0 - D10 - SS
    _BV(4), // PB4 - D11 -
    _BV(3), // PB3 - D12 - MISO
    _BV(1), // PB1 - D13 - LED13  -  SCK

    _BV(0), // PD0 - D14 - SCL
    _BV(1), // PD1 - D15 - SDA

    _BV(7), // PE7 - D16 - SW_N
    _BV(6), // PE6 - D17 - SW_E
    _BV(5), // PE5 - D18 - SW_S
    _BV(4), // PE4 - D19 - SW_W
    _BV(3), // PE3 - D20 - LCD_BL
    _BV(2), // PE2 - D21 - LCD_E
    _BV(1), // PE1 - D22 - LCD_RW
    _BV(0), // PE0 - D23 - LCD_RS

    _BV(7), // PF7 - D24 - A0
    _BV(6), // PF6 - D25 - A1
    _BV(5), // PF5 - D26 - A2
    _BV(4), // PF4 - D27 - A3
    _BV(3), // PF3 - D28 - A4
    _BV(2), // PF2 - D29 - A5
    _BV(1), // PF1 - D30 - A6
    _BV(0), // PF0 - D31 - A7

    _BV(0), // PA0 - D32 - LED0   - LCD_DB0
    _BV(1), // PA1 - D33 - LED1   - LCD_DB1
    _BV(2), // PA2 - D34 - LED2   - LCD_DB2
    _BV(3), // PA3 - D35 - LED3   - LCD_DB3
    _BV(4), // PA4 - D36 - LED4   - LCD_DB4
    _BV(5), // PA5 - D37 - LED5   - LCD_DB5
    _BV(6), // PA6 - D38 - LED6   - LCD_DB6
    _BV(7), // PA7 - D39 - LED7   - LCD_DB7

    _BV(0), // PC0 - D40 - SERVO_1
    _BV(1), // PC1 - D41 - SERVO_2
    _BV(2), // PC2 - D42 - 4A
    _BV(3), // PC3 - D43 - 2A
    _BV(4), // PC4 - D44 - 3A
    _BV(5), // PC5 - D45 - 1A
    _BV(6), // PC6 - D46 - BUZZER
    _BV(7), // PC7 - D47 - SW_C

};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
    NOT_ON_TIMER, // PD2 - D0  - RX1
    NOT_ON_TIMER, // PD3 - D1  - TX1

    NOT_ON_TIMER, // PB2 - D2  - MOSI 
    TIMER1C,      // PB7 - D3  -

    NOT_ON_TIMER, // PD7 - D4  -

    TIMER1B,      // PB6 - D5  -
    TIMER1A,      // PB5 - D6  -

    NOT_ON_TIMER, // PD6 - D7  -
    NOT_ON_TIMER, // PD5 - D8  -
    NOT_ON_TIMER, // PD4 - D9  -

    NOT_ON_TIMER, // PB0 - D10 - SS
    TIMER2A,      // PB4 - D11 -
    NOT_ON_TIMER, // PB3 - D12 - MISO
    NOT_ON_TIMER, // PB1 - D13 - LED13  -  SCK

    TIMER0B,      // PD0 - D14 - SCL
    TIMER2B,      // PD1 - D15 - SDA

    NOT_ON_TIMER, // PE7 - D16 - SW_N
    NOT_ON_TIMER, // PE6 - D17 - SW_E
    NOT_ON_TIMER, // PE5 - D18 - SW_S
    NOT_ON_TIMER, // PE4 - D19 - SW_W
    NOT_ON_TIMER, // PE3 - D20 - LCD_BL
    NOT_ON_TIMER, // PE2 - D21 - LCD_E
    NOT_ON_TIMER, // PE1 - D22 - LCD_RW
    NOT_ON_TIMER, // PE0 - D23 - LCD_RS

    NOT_ON_TIMER, // PF7 - D24 - A0
    NOT_ON_TIMER, // PF6 - D25 - A1
    NOT_ON_TIMER, // PF5 - D26 - A2
    NOT_ON_TIMER, // PF4 - D27 - A3
    NOT_ON_TIMER, // PF3 - D28 - A4
    NOT_ON_TIMER, // PF2 - D29 - A5
    NOT_ON_TIMER, // PF1 - D30 - A6
    NOT_ON_TIMER, // PF0 - D31 - A7

    NOT_ON_TIMER, // PA0 - D32 - LED0   - LCD_DB0
    NOT_ON_TIMER, // PA1 - D33 - LED1   - LCD_DB1
    NOT_ON_TIMER, // PA2 - D34 - LED2   - LCD_DB2
    NOT_ON_TIMER, // PA3 - D35 - LED3   - LCD_DB3
    NOT_ON_TIMER, // PA4 - D36 - LED4   - LCD_DB4
    NOT_ON_TIMER, // PA5 - D37 - LED5   - LCD_DB5
    NOT_ON_TIMER, // PA6 - D38 - LED6   - LCD_DB6
    NOT_ON_TIMER, // PA7 - D39 - LED7   - LCD_DB7

    NOT_ON_TIMER, // PC0 - D40 - SERVO_1
    NOT_ON_TIMER, // PC1 - D41 - SERVO_2
    NOT_ON_TIMER, // PC2 - D42 - 4A
    NOT_ON_TIMER, // PC3 - D43 - 2A
    TIMER3C, // PC4 - D44 - 3A
    TIMER3B, // PC5 - D45 - 1A
    TIMER3A, // PC6 - D46 - BUZZER
    NOT_ON_TIMER, // PC7 - D47 - SW_C

};

const uint8_t PROGMEM analog_pin_to_channel_PGM[] = {
  7,  // A0 PF7
  6,  // A1 PF6 
  5,  // A2 PF5 
  4,  // A3 PF4
  3,  // A4 PF3 
  2,  // A5 PF2   
  1,  // A6 PD1 
  0,  // A7 PD0 
};

#endif /* ARDUINO_MAIN */
#endif /* Pins_Arduino_h */