/*
 * Dwengo board configuration
 * part of Dwengo library
 *
 * Dwengo board settings, also supports WELEK version
 *
 * Version: 1.0.$Revision: 4607 $
 * Date: $Date: 2011-10-14 08:53:52 +0200 (Fri, 14 Oct 2011) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */
/**
 * \file
 * \brief Dwengo board configuration
 *
 * Dwengo board settings, also supports WELEK version
 */
 
/**
 * \mainpage Dwengo library
 * The goal of the Dwengo library is to make programming 
 * the Dwengo board as easy as possible. The library hides the low level details of the PIC18F4550 
 * microcontroller, allowing the programmer to focus on the design of the algorithm. The dwengoBoard.h 
 * file contains many handy defines and the initBoard() function. Including dwengoConfig.h configures
 * the PIC18F4550 to the default Dwengo configuration (48MHz). Besides this, the library contains 
 * functionality to ease the use of the ADC (dwengoADC.h), the USART (dwengoUsart.h), servos 
 * (dwengoServo.h), motors (dwengoMotor.h), the LCD (dwengoLCD.h), a remote control (dwengoRC.h) 
 * and delays (dwengoDelay.h). Note that the aim of the library is not to get maximum performance 
 * out of the Dwengo board, but to make programming the Dwengo board easy and accessible to everyone.
 * 
 * \section temp Program template
 * The following code shows how to use the Dwengo library.
 *
 * \code
 * #include <dwengoBoard.h>
 * #include <dwengoConfig.h>
 * #include <dwengoADC.h>
 * #include <dwengoMotor.h>
 * // Include other files here... 
 * 
 * void main(void) {
 *   // Declare variables here...
 *
 *   initBoard();
 *   initADC();
 *   initMotor();
 *   // Initialise other functionality here...
 *
 *   // The main loop goes here...
 * 
 * }
 * \endcode
 *
 * \section mplab Using the Dwengo library in MPLAB
 *
 * Download the dwengo-lib.zip and extract it. Copy the header files (*.h) in the h-directory of your
 * MCC18 installation (usually C:\\MCC18\\h). Copy dwengo.lib in the lib directory of your MCC18 
 * installation (usually C:\\MCC18\\lib).
 *
 * You must add dwengo.lib to every project in which you want to make use of the functionality of 
 * the Dwengo library. You can do this in the following way. Click Project>Add Files to Project... 
 * Browse to C:\\MCC18\\lib\\, select dwengo.lib and click Open.
 *
 */

#ifndef DWENGO_BOARD_H
#define DWENGO_BOARD_H

// Constants
#define TRUE    1
#define FALSE   0
#define HIGH    1
#define LOW     0
#define PRESSED 0
#define INPUT   1
#define OUTPUT  0

// Data types
typedef unsigned char BYTE;

// Pin assignments

// leds
#define LEDS                PORTD
#define LEDS_DIR            TRISD
#define LED0                PORTDbits.RD0
#define LED1                PORTDbits.RD1
#define LED2                PORTDbits.RD2
#define LED3                PORTDbits.RD3
#define LED4                PORTDbits.RD4
#define LED5                PORTDbits.RD5
#define LED6                PORTDbits.RD6
#define LED7                PORTDbits.RD7

// buttons
#if defined(WELEK)
  #define SW_C                PORTBbits.RB0
  #define SW_C_DIR            TRISBbits.TRISB0
  #define SW_N                PORTBbits.RB4
  #define SW_N_DIR            TRISBbits.TRISB4
  #define SW_E                PORTBbits.RB7
  #define SW_E_DIR            TRISBbits.TRISB7
  #define SW_S                PORTBbits.RB5
  #define SW_S_DIR            TRISBbits.TRISB5
  #define SW_W                PORTBbits.RB6
  #define SW_W_DIR            TRISBbits.TRISB6
#else
  #define SW_C                PORTBbits.RB2
  #define SW_C_DIR            TRISBbits.TRISB2
  #define SW_N                PORTBbits.RB4
  #define SW_N_DIR            TRISBbits.TRISB4
  #define SW_E                PORTBbits.RB1
  #define SW_E_DIR            TRISBbits.TRISB1
  #define SW_S                PORTBbits.RB5
  #define SW_S_DIR            TRISBbits.TRISB5
  #define SW_W                PORTBbits.RB0
  #define SW_W_DIR            TRISBbits.TRISB0
#endif

#define isPressedSW_N()		(SW_N==PRESSED)
#define isPressedSW_E()		(SW_E==PRESSED)
#define isPressedSW_S()		(SW_S==PRESSED)
#define isPressedSW_W()		(SW_W==PRESSED)
#define isPressedSW_C()		(SW_C==PRESSED)


// LCD display
#define LCD_DATA            PORTD 
#define LCD_DATA_DIR        TRISD
#define LCD_BACKLIGHT       PORTCbits.RC0
#define LCD_BACKLIGHT_DIR   TRISCbits.TRISC0
#define LCD_RW              PORTEbits.RE1
#define LCD_RW_DIR          TRISEbits.TRISE1
#define LCD_RS              PORTEbits.RE0
#define LCD_RS_DIR          TRISEbits.TRISE0
#define LCD_EN              PORTEbits.RE2
#define LCD_EN_DIR          TRISEbits.TRISE2

// servo connectors
#define SERVO1              LATBbits.LATB5
#define SERVO1_DIR          TRISBbits.TRISB5
#define SERVO2              LATBbits.LATB4
#define SERVO2_DIR          TRISBbits.TRISB4

// motor driver
#if defined(WELEK)
  #define MOTOR1_0            PORTCbits.RC2
  #define MOTOR1_0_DIR        TRISCbits.TRISC2
  #define MOTOR1_1            PORTBbits.RB2
  #define MOTOR1_1_DIR        TRISBbits.TRISB2
  #define MOTOR2_0            PORTCbits.RC1
  #define MOTOR2_0_DIR        TRISCbits.TRISC1
  #define MOTOR2_1            PORTBbits.RB3
  #define MOTOR2_1_DIR        TRISBbits.TRISB3
#else
  #define MOTOR1_0            PORTCbits.RC2
  #define MOTOR1_0_DIR        TRISCbits.TRISC2
  #define MOTOR1_1            PORTAbits.RA4
  #define MOTOR1_1_DIR        TRISAbits.TRISA4
  #define MOTOR2_0            PORTCbits.RC1
  #define MOTOR2_0_DIR        TRISCbits.TRISC1
  #define MOTOR2_1            PORTBbits.RB3
  #define MOTOR2_1_DIR        TRISBbits.TRISB3
#endif

// include libraries
#include <p18f4550.h>
#include "dwengoDelay.h"

#ifndef NO_LCD
#include "dwengoLCD.h"
#endif
/**
 * \brief Initialize Dwengo board
 *
 * Initialization routine for the Dwengo board. Enables pull-ups for PORT B, 
 * enables all switches and sets LED pins as outputs. Additionally the LCD is initialized by this function.
 */
void initBoard(void);

/**
 * \brief Set all LEDs at once
 *
 * Set all LEDs at once
 *
 * @param led  value that you want to initialize PORTD (LEDS) with
 */
void setLedRegister(BYTE led);


/**
 * \brief Set specific LED
 *
 * Set a specific LED
 *
 * @param position  position of the LED to set
 */
void setLed(BYTE position);

/**
 * \brief Clear specific LED
 *
 * Clears a specific LED
 *
 * @param position  position of the LED to set
 */
void clearLed(BYTE position);

/**
 * \brief Set LED pattern
 *
 * Set a specific LED pattern
 *
 * @param ld7  state of led 7 (0 is off, 1 is on)
 * @param ld6  state of led 6 (0 is off, 1 is on)
 * @param ld5  state of led 5 (0 is off, 1 is on)
 * @param ld4  state of led 4 (0 is off, 1 is on)
 * @param ld3  state of led 3 (0 is off, 1 is on)
 * @param ld2  state of led 2 (0 is off, 1 is on)
 * @param ld1  state of led 1 (0 is off, 1 is on)
 * @param ld0  state of led 0 (0 is off, 1 is on)
 */
void setLedPattern(BYTE ld7,BYTE ld6,BYTE ld5,BYTE ld4,BYTE ld3,BYTE ld2,BYTE ld1,BYTE ld0);

#endif //DWENGO_BOARD_H
