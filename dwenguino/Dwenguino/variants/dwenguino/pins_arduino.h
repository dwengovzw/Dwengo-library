/*
  pins_arduino.h - Pin definition functions for Arduino adapted to work with Dwenguino v2.0
  Part of the Dwengo library

  Originally made by David A. Mellis,

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

/*
  Modified on Dec 20 2014 by Jelle Roets from Dwengo vzw (www.dwengo.org)
    Pin definition for dwenguino v2.0

  TODO:
    - make arduino core ready for AT90USB646  (wiring_private.h: EXTERNAL_NUM_INTERRUPTS, WInterrupts.c: define interrupts, Wiring )
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

// Libraries control:
#define TONE_USE_TIMER2         // Use timer 2 for Tone, You can also choose to use timer 1 or 3
#define DISABLE_NEWPING_TIMER   // disable timer functionality of newpin library because they use same timer2 in CTC mode as tone function

#define NUM_DIGITAL_PINS  48
#define NUM_ANALOG_INPUTS 8

// No TX or RX led connected 
#define TX_RX_LED_INIT	//DDRD |= (1<<5), DDRB |= (1<<0)
#define TXLED0			//PORTD |= (1<<5)
#define TXLED1			//PORTD &= ~(1<<5)
#define RXLED0			//PORTB |= (1<<0)
#define RXLED1			//PORTB &= ~(1<<0)
    
static const uint8_t RX = 0;
static const uint8_t TX = 1;

static const uint8_t SDA = 15;
static const uint8_t SCL = 14;

// Map SPI port to 'new' pins 
static const uint8_t SS   = 10;
static const uint8_t MOSI = 2;
static const uint8_t MISO = 12;
static const uint8_t SCK  = 13;

// Mapping of analog pins as digital I/O
static const uint8_t A0 = 24;
static const uint8_t A1 = 25;
static const uint8_t A2 = 26;
static const uint8_t A3 = 27;
static const uint8_t A4 = 28;
static const uint8_t A5 = 29;
static const uint8_t A6 = 30; 
static const uint8_t A7 = 31;

static const uint8_t BAND_GAP = 70; 
static const uint8_t GROUND = 71; // = 31 - 8 + 48 (for more info see analog_pin_to_channel_PGM at the end of this file)

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

// External interrupts
static const uint8_t D0_INT= 2;   // INT2 on PD2 - D0
static const uint8_t D1_INT= 3;   // INT3 on PD3 - D1
static const uint8_t D14_INT = 0; // INT0 on PD0 - D14
static const uint8_t D15_INT = 1; // INT1 on PD1 - D15
static const uint8_t D16_INT = 7; // INT7 on PE7 - D16
static const uint8_t D17_INT = 6; // INT6 on PE6 - D17
static const uint8_t D18_INT = 5; // INT5 on PE5 - D18
static const uint8_t D19_INT = 4; // INT4 on PE4 - D19

// PCINT 0-7 are on pins 6-13
#define digitalPinToPCICR(p)    ( (((p) >= 6) && ((p) <= 13)) ? (&PCICR) : ((uint8_t *)0) )
#define digitalPinToPCICRbit(p) 0
#define digitalPinToPCMSK(p)    ( (((p) >= 6) && ((p) <= 13)) ? (&PCMSK0) : ((uint8_t *)0) )
#define digitalPinToPCMSKbit(p) ( (((p) >= 6) && ((p) <= 13)) ? ((p) - 6) : 0 )

extern const uint8_t PROGMEM analog_pin_to_channel_PGM[];
#define analogPinToChannel(P)  ( pgm_read_byte( analog_pin_to_channel_PGM + (P) ) )

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

// Converts pinNumber to MUX channel bits (MUX4..0 datasheet p322)
// First 48 definitions map digital IO pins to ADC channel: digital pins without ADC channel will be read as ground (MUX = 31),
// last defintions can be used for special ADC commamds (like internal bandgap or differential ADC)
const uint8_t PROGMEM analog_pin_to_channel_PGM[] = {
    7, // readAnalog(0) => ADC7 (first 7 idices are not digital ports but number of analog channel)
    6, // readAnalog(1) => ADC6 
    5, // readAnalog(2) => ADC5 
    4, // readAnalog(3) => ADC4  
    3, // readAnalog(4) => ADC3 
    2, // readAnalog(5) => ADC2 
    1, // readAnalog(6) => ADC1 
    0, // readAnalog(7) => ADC0

    31, // PD5 - D8: readAnalog(8) => read as ground
    31, // PD4 - D9  -
    31, // PB0 - D10 - SS
    31, // PB4 - D11 -
    31, // PB3 - D12 - MISO
    31, // PB1 - D13 - LED13  -  SCK
    31, // PD0 - D14 - SCL
    31, // PD1 - D15 - SDA

    31, // PE7 - D16 - SW_N
    31, // PE6 - D17 - SW_E
    31, // PE5 - D18 - SW_S
    31, // PE4 - D19 - SW_W
    31, // PE3 - D20 - LCD_BL
    31, // PE2 - D21 - LCD_E
    31, // PE1 - D22 - LCD_RW
    31, // PE0 - D23 - LCD_RS

    7, // PF7 - D24 - A0: readAnalog(A0) => ADC7
    6, // PF6 - D25 - A1: readAnalog(A1) => ADC6
    5, // PF5 - D26 - A2: readAnalog(A2) => ADC5
    4, // PF4 - D27 - A3: readAnalog(A3) => ADC4
    3, // PF3 - D28 - A4: readAnalog(A4) => ADC3
    2, // PF2 - D29 - A5: readAnalog(A5) => ADC2
    1, // PF1 - D30 - A6: readAnalog(A6) => ADC1
    0, // PF0 - D31 - A7: readAnalog(A7) => ADC0

    31, // PA0 - D32 - LED0   - LCD_DB0
    31, // PA1 - D33 - LED1   - LCD_DB1
    31, // PA2 - D34 - LED2   - LCD_DB2
    31, // PA3 - D35 - LED3   - LCD_DB3
    31, // PA4 - D36 - LED4   - LCD_DB4
    31, // PA5 - D37 - LED5   - LCD_DB5
    31, // PA6 - D38 - LED6   - LCD_DB6
    31, // PA7 - D39 - LED7   - LCD_DB7

    31, // PC0 - D40 - SERVO_1
    31, // PC1 - D41 - SERVO_2
    31, // PC2 - D42 - 4A
    31, // PC3 - D43 - 2A
    31, // PC4 - D44 - 3A
    31, // PC5 - D45 - 1A
    31, // PC6 - D46 - BUZZER
    31, // PC7 - D47 - SW_C

    8, // array idx = 48: readAnalog(48) => (ADC0 / ADC0 / 10x)
    9, 
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
    30, // array idx = 70: readAnalog(BAND_GAP) => Band gap 1.1V
    31  // array idx = 71: readAnalog(GROUND)  => Ground 0V
};

#endif /* ARDUINO_MAIN */

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR        Serial
#define SERIAL_PORT_USBVIRTUAL     Serial
#define SERIAL_PORT_HARDWARE       Serial1
#define SERIAL_PORT_HARDWARE_OPEN  Serial1

#endif /* Pins_Arduino_h */
