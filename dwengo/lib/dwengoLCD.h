/*
 * LCD Display
 * part of Dwengo library
 *
 * Control of LCD display on the Dwengo board
 *
 * Version: 1.0.$Revision: 2141 $
 * Date: $Date: 2010-05-04 21:26:28 +0200 (Tue, 04 May 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */
/**
 * \file
 * \brief LCD Display
 *
 * Control of LCD display on the Dwengo board
 */
#ifndef DWENGO_LCD_H
#define DWENGO_LCD_H

#include "dwengoBoard.h"
#include "dwengoDelay.h"

// Properties
#define LCD_WIDTH       16                /**< Number of columns on the LCD */
#define LCD_HEIGHT      2                 /**< Number of lines on the LCD */
#define LCD_LASTLINE    (LCD_HEIGHT - 1)  /**< Line number of the last line of the LCD */
#define LCD_LASTPOS     (LCD_WIDTH - 1)   /**< Column number of the last column of the LCD */

// Macro's
/**
 * \brief Enable LCD backlight
 *
 */
#define backlightOn()    (LCD_BACKLIGHT = 1)
/**
 * \brief Disable LCD backlight
 *
 */
#define backlightOff()   (LCD_BACKLIGHT = 0)
/**
 * \brief Write a string to the LCD display
 *
 * The string is written to the current cursor position of the LCD display, and the cursor is incremented.
 *
 * @param message the NULL-terminated ASCII string to write
 */
#define appendStringToLCD(message)   appendStringToLCD_((const FAR_ROM char*)(message))

// Functions
/**
 * \brief Initialize LCD display
 *
 * Sets the display to use 8-bit interface, turns the blinking cursor off, ... TODO
 *
 * Must be called when the LCD is connected, even when it is not in use,
 * to avoid the display ""doing strange things with its pins""
 */
void initLCD(void);
/**
 * \brief Clears the LCD display
 */
void clearLCD(void);
/**
 * \brief Send command to LCD display
 *
 * Also saves and restores the state of the LEDS, and waits for the command to be processed by the LCD.
 * @param commandData the command, see the datasheet of the HD45... controller for a list of possible commands
 */
void commandLCD(const BYTE commandData);
/**
 * \brief Set cursor to the desired position
 *
 * Set cursor to desired position. Append functions will start from this location.
 * @param l cursor line can be 0 or 1
 * @param p cursor position between 0 and 15
 */
void setCursorLCD(BYTE l, BYTE p);
/**
 * \brief Write a character to the LCD display
 *
 * Character is written to the current cursor position of the LCD display, and the cursor is incremented.
 * Line wrapping is automatic, assuming a line size of #LCD_WIDTH characters.
 *
 * This function assumes a 2-lines display
 *
 * @param c the ASCII character to write
 */
void appendCharToLCD(const char c);
/**
 * \brief Write character to a desired position
 *
 * Character is written to a position given by the the user.
 * @param c the ASCII character to write
 * @param l cursor line can be 0 or 1
 * @param p cursor position between 0 and 15
 */
void printCharToLCD(const char c, BYTE l, BYTE p);
/**
 * \brief Write a ROM string to the LCD display
 *
 * The string is written to the current cursor position of the LCD display, and the cursor is incremented.
 *
 * @param message the NULL-terminated ASCII string to write
 */
void appendStringToLCD_(const FAR_ROM char* message);
/**
 * \brief Write a ROM string to a desired position on the LCD display
 *
 * The string is written to a desired position of the LCD display, and the cursor is incremented.
 *
 * @param message the NULL-terminated ASCII string to write
 * @param l cursor line can be 0 or 1
 * @param p cursor position between 0 and 15
 */
void printStringToLCD(char* message, BYTE l, BYTE p);
/**
 * \brief Write an integer to the LCD display
 *
 * Converts an integer to an ASCII string and writes it to the current cursor position of the LCD display,
 * incrementing the cursor.
 *
 * @param i the signed integer to write
 */
void appendIntToLCD(int i);
/**
 * \brief Write an integer to a desired position on the LCD display
 *
 * Converts an integer to an ASCII string and writes it to a user defined cursor position of the LCD display.
 *
 * @param i the signed integer to write
 * @param l cursor line can be 0 or 1
 * @param p cursor position between 0 and 15
 */
void printIntToLCD(int i, BYTE l, BYTE p);

#endif // DWENGO_LCD_H
