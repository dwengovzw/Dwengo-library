/*
 * Delay
 * part of Dwengo library
 *
 * Delay routines for the dwengo board.
 *
 * Version: 1.0.$Revision: 1933 $
 * Date: $Date: 2010-03-14 21:43:25 +0100 (Sun, 14 Mar 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */
 /**
  * \file
  * \brief Delay routines
  *
  * Implementation of delay functionality for the Dwengo board
  */

#ifndef DWENGO_DELAY_H
#define DWENGO_DELAY_H

/**
 * \brief Delay in microseconds
 *
 * @param n number (0-255) of microseconds to wait
 */
#ifdef SDCC_pic16
//fix problem with sdcc-sdcpp not inserting newlines in macros
void delay_us(char a);

#else
#define delay_us(n)       \
{	char a=n;  \
    _asm                  \
	MOVLW a               \
	MOVF PLUSW2, 1, 0     \
	BZ   12               \
	DECF PLUSW2, 1, 0     \
loop:                     \
	MOVLW a               \
    MOVF PLUSW2, 1, 0     \
	BZ   8                \
	NOP                   \
	NOP                   \
	NOP                   \
	NOP                   \
	NOP                   \
	NOP                   \
	DECF PLUSW2, 1, 0     \
	BRA  -11              \
end:                      \
	_endasm }
#endif
/**
 * \brief Delay in 100 microseconds
 *
 * @param n times 100 microseconds the microcontroller has to wait
 */
void delay_100us(int n);
/**
 * \brief Delay in milliseconds
 *
 * @param n number of milliseconds the program has to wait
 */
void delay_ms(int n);
/**
 * \brief Delay in seconds
 *
 * @param n times seconds the microcontroller has to wait
 */
void delay_s(int n);

#endif // DWENGO_DELAY_H
