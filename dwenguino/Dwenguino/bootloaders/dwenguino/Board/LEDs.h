/*
LUFA Library
Copyright (C) Dean Camera, 2017.

dean [at] fourwalledcubicle [dot] com
www.lufa-lib.org
*/

/*
Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)

Permission to use, copy, modify, distribute, and sell this
software and its documentation for any purpose is hereby granted
without fee, provided that the above copyright notice appear in
all copies and that both that the copyright notice and this
permission notice and warranty disclaimer appear in supporting
documentation, and that the name of the author not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

The author disclaims all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall the author be liable for any
special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
*/

/*
Copyright (c) 2017, aweatherguy
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
Neither the name of the <organization> nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/** \file
*  \brief Board specific LED driver header for the Arduino Leonardo board.
*  \copydetails Group_LEDs_LEONARDO
*
*  \note This file should not be included directly. It is automatically included as needed by the LEDs driver
*        dispatch header located in LUFA/Drivers/Board/LEDs.h.
*/

/** \ingroup Group_LEDs
*  \defgroup Group_LEDs_LEONARDO LEONARDO
*  \brief Board specific LED driver header for the Arduino Leonardo board.
*
*  Board specific LED driver header for the Arduino Leonardo board (http://arduino.cc/en/Main/arduinoBoardLeonardo).
*
*  <table>
*    <tr><th>Name</th><th>Color</th><th>Info</th><th>Active Level</th><th>Port Pin</th></tr>
*    <tr><td>LEDS_LED1</td><td>Yellow</td><td>RX</td><td>Low</td><td>PORTB.0</td></tr>
*    <tr><td>LEDS_LED2</td><td>Yellow</td><td>TX</td><td>Low</td><td>PORTD.5</td></tr>
*    <tr><td>LEDS_LED3</td><td>Yellow</td><td>General Indicator</td><td>High</td><td>PORTC.7</td></tr>
*  </table>
*
*  @{
*/

#pragma once

/* Includes: */
#include "../../LUFA/Common/Common.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
// 
// Edit to specify the PORT, DDR and BIT for each of the three LEDs
// Also specify the bit value when the LED is turned on..."1" or "0" ?
//
#if 0
// Leonardo
#define LEDS_RX_PORT	PORTB
#define LEDS_TX_PORT	PORTD
#define LEDS_L_PORT	PORTC

#define LEDS_RX_DDR	DDRB
#define LEDS_TX_DDR	DDRD
#define LEDS_L_DDR	DDRC

#define LEDS_RX_BIT	0x01
#define LEDS_TX_BIT	0x20
#define LEDS_L_BIT	0x80

#define LEDS_RX_ACTIVE  0
#define LEDS_TX_ACTIVE  0
#define LEDS_L_ACTIVE   1

#else
// WxRecorder (LED_L not implemented)

#define LEDS_RX_PORT	PORTB
#define LEDS_TX_PORT	PORTC
#define LEDS_L_PORT	PORTB

#define LEDS_RX_DDR	DDRB
#define LEDS_TX_DDR	DDRC
#define LEDS_L_DDR	DDRB

#define LEDS_RX_BIT	0x40
#define LEDS_TX_BIT	0x40
#define LEDS_L_BIT	0x10
    
#define LEDS_RX_ACTIVE  1
#define LEDS_TX_ACTIVE  1
#define LEDS_L_ACTIVE   1

#endif

// ===============================================================
// =============== END OF USER CONFIGURATION SECTION =============
// ===============    DO NOT EDIT BELOW THIS LINE    =============
// ===============================================================

#if LEDS_RX_ACTIVE
#define LEDS_RX_ON	|=
#define LEDS_RX_OFF	&= ~
#define LEDS_RX_TEST ((LEDS_RX_PORT & ~LEDS_RX_BIT) != 0)
#else
#define LEDS_RX_OFF	|=
#define LEDS_RX_ON	&= ~
#define LEDS_RX_TEST ((LEDS_RX_PORT & ~LEDS_RX_BIT) == 0)
#endif

#if LEDS_TX_ACTIVE
#define LEDS_TX_ON	|=
#define LEDS_TX_OFF	&= ~
#define LEDS_TX_TEST ((LEDS_TX_PORT & ~LEDS_TX_BIT) != 0)
#else
#define LEDS_TX_OFF	|=
#define LEDS_TX_ON	&= ~
#define LEDS_TX_TEST ((LEDS_TX_PORT & ~LEDS_TX_BIT) == 0)
#endif

#if LEDS_L_ACTIVE
#define LEDS_L_ON	|=
#define LEDS_L_OFF	&= ~
#define LEDS_L_TEST ((LEDS_L_PORT & ~LEDS_L_BIT) != 0)
#else
#define LEDS_L_OFF	|=
#define LEDS_L_ON	&= ~
#define LEDS_L_TEST ((LEDS_L_PORT & ~LEDS_L_BIT) == 0)
#endif

/** 
Public Interface - May be used in end-application.
These functions are purposely NOT inline to save flash bytes.
*/
   
#if !defined(__DOXYGEN__)

    inline void LEDs_Init(void)
    {
#if defined LED_DATA_FLASHES
	LEDS_RX_DDR |= LEDS_RX_BIT;
	LEDS_RX_PORT LEDS_RX_OFF LEDS_RX_BIT;

	LEDS_TX_DDR |= LEDS_TX_BIT;
	LEDS_TX_PORT LEDS_TX_OFF LEDS_TX_BIT;
#endif

#if LED_START_FLASHES > 0
	LEDS_L_DDR |= LEDS_L_BIT;
	LEDS_L_PORT LEDS_L_OFF LEDS_L_BIT;
#endif
    }

    inline void LEDs_Disable(void)
    {
	/** 
	Set everything back to original state -- all DDR bits to input, and all port bits to zero.
	Don't set DDRs/PORTs multiple times if LEDs have a port in common.
	*/
	
#if defined LED_DATA_FLASHES
	LEDS_RX_DDR = 0;
	LEDS_RX_PORT = 0;

	LEDS_TX_DDR = 0;
	LEDS_TX_PORT = 0;
#endif

#if LED_START_FLASHES > 0
	LEDS_L_DDR = 0;
	LEDS_L_PORT = 0;
#endif
    }
    
    inline void LEDs_RX_Off(void)
    {
#if defined LED_DATA_FLASHES
	LEDS_RX_PORT LEDS_RX_OFF LEDS_RX_BIT;
#endif
    }

    inline void LEDs_RX_On(void)
    {
#if defined LED_DATA_FLASHES
	LEDS_RX_PORT LEDS_RX_ON LEDS_RX_BIT;
#endif
    }

    inline void LEDs_RX_Toggle(void)
    {
#if defined LED_DATA_FLASHES
	LEDS_RX_PORT ^= LEDS_RX_BIT;
#endif
    }

    inline void LEDs_TX_Off(void)
    {
#if defined LED_DATA_FLASHES
	LEDS_TX_PORT LEDS_TX_OFF LEDS_TX_BIT;
#endif
    }

    inline void LEDs_TX_On(void)
    {
#if defined LED_DATA_FLASHES
	LEDS_TX_PORT LEDS_TX_ON LEDS_TX_BIT;
#endif
    }

    inline void LEDs_TX_Toggle(void)
    {
#if defined LED_DATA_FLASHES
	LEDS_TX_PORT ^= LEDS_TX_BIT;
#endif
    }

    inline void LEDs_L_Off(void)
    {
#if LED_START_FLASHES > 0
	LEDS_L_PORT LEDS_L_OFF LEDS_L_BIT;
#endif
    }

    inline void LEDs_L_On(void)
    {
#if LED_START_FLASHES > 0
	LEDS_L_PORT LEDS_L_ON LEDS_L_BIT;
#endif
    }

    inline void LEDs_L_Toggle(void)
    {
#if LED_START_FLASHES > 0
	LEDS_L_PORT ^= LEDS_L_BIT;
#endif
    }

#endif  // !defined(__DOXYGEN__)

    /* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
/** @} */

