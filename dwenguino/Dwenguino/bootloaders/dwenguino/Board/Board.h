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

/** \file
 *  \brief Board specific information header for the Arduino Leonardo board.
 *  \copydetails Group_BoardInfo_WXRECORDER
 *
 *  \note This file should not be included directly. It is automatically included as needed by the Board driver
 *        dispatch header located in LUFA/Drivers/Board/Board.h.
 */

/** \ingroup Group_BoardInfo
 *  \defgroup Group_BoardInfo_WXRECORDER WXRECORDER
 *  \brief Board specific information header for the Arduino Leonardo board.
 *
 *  Board specific information header for the Arduino Leonardo board (http://arduino.cc/en/Main/arduinoBoardLeonardo).
 *
 *  @{
 */

/*
* Modified on Dec 23 2017 by Jelle Roets from Dwengo vzw (www.dwengo.org)
*   Bootloader for Dwenguino
*
* Changes made:
*   - add all dwenguino specific board definitions here (including Leds + remove seperate leds.h file)
*/

#pragma once

    /* Includes: */
    #include <LUFA/Common/Common.h>

    /* Enable C linkage for C++ Compilers: */
    #if defined(__cplusplus)
      extern "C" {
    #endif

    /* Preprocessor Checks: */
    #if !defined(__INCLUDE_FROM_BOARD_H)
      #error Do not include this file directly. Include LUFA/Drivers/Board/Board.h instead.
    #endif


// LEDS
    #define LED13_PORT PORTB   // LED 13 on PB1
    #define LED13_DDR  DDRB
    #define LED13_BIT  (1<<1)

    static inline void LEDs_Init(void) {
        LED13_DDR  |=  LED13_BIT;
    }
    static inline void LEDs_Disable(void) {
        LED13_DDR  &= ~LED13_BIT;
    }
    static inline void LED13_Off(void) {
        LED13_PORT &= ~LED13_BIT;
    }
    static inline void LED13_On(void) {
        LED13_PORT |= LED13_BIT;
    } 
    static inline void LED13_Toggle(void) {
        LED13_PORT ^= LED13_BIT;
    }

// Buttons
    #define SWC_DDR   DDRC  
    #define SWC_PORT  PORTC
    #define SWC_PIN   PINC
    #define SWC_BIT   (1<<7) // Switch center on PC7
    #define SW_DDR    DDRE
    #define SW_PORT   PORTE
    #define SW_PIN    PINE
    #define SWN_BIT   (1<<7) // Switch Noth on PE7
    #define SWE_BIT   (1<<6) // Switch Noth on PE6
    #define SWS_BIT   (1<<5) // Switch Noth on PE5
    #define SWW_BIT   (1<<4) // Switch Noth on PE4


// configure buttons as input with pull-up
    static inline void SWC_init(void){
        SWC_DDR &= ~SWC_BIT;
        SWC_PORT |= SWC_BIT;
    }
    static inline void SWN_init(void){
        SW_DDR &= ~SWN_BIT;
        SW_PORT |= SWN_BIT;
    }
    static inline void SWE_init(void){
        SW_DDR &= ~SWE_BIT;
        SW_PORT |= SWE_BIT;
    }
    static inline void SWS_init(void){
        SW_DDR &= ~SWS_BIT;
        SW_PORT |= SWS_BIT;
    }
    static inline void SWW_init(void){
        SW_DDR &= ~SWW_BIT;
        SW_PORT |= SWW_BIT;
    }

    static inline uint8_t SWC_pressed(void){
        return !(SWC_PIN & SWC_BIT);
    }
    static inline uint8_t SWN_pressed(void){
        return !(SW_PIN & SWN_BIT);
    }
    static inline uint8_t SWE_pressed(void){
        return !(SW_PIN & SWE_BIT);
    }
    static inline uint8_t SWS_pressed(void){
        return !(SW_PIN & SWS_BIT);
    }
    static inline uint8_t SWW_pressed(void){
        return !(SW_PIN & SWW_BIT);
    }

#if defined (BOOT_SWC)
    #define BOOTSW_init() SWC_init()
    #define BOOTSW_pressed() SWC_pressed()
#elif defined (BOOT_SWN)
    #define BOOTSW_init() SWN_init()
    #define BOOTSW_pressed() SWN_pressed()
#elif defined (BOOT_SWE)
    #define BOOTSW_init() SWE_init()
    #define BOOTSW_pressed() SWE_pressed()
#elif defined (BOOT_SWS)
    #define BOOTSW_init() SWS_init()
    #define BOOTSW_pressed() SWS_pressed()
#elif defined (BOOT_SWW)
    #define BOOTSW_init() SWW_init()
    #define BOOTSW_pressed() SWW_pressed()
#endif
    // #define SWC_SETUP()   {DDRC &= ~(1<<7); PORTC |= (1<<7);} // configure Switch C pin (=PC7) as input with pull-up
    // #define SWC_STATUS()  (PINC & (1<<7))


/* Disable C linkage for C++ Compilers: */
  #if defined(__cplusplus)
}
  #endif


/** @} */