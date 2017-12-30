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

  Header file for the Katiana bootloader.
*/

/*
* Modified on Dec 23 2017 by Jelle Roets from Dwengo vzw (www.dwengo.org)
*   Bootloader for Dwenguino
*
* Changes made:
*   - specify Dwengo Bootloader version
*   
*/

#pragma once

/* Includes: */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "Descriptors.h"
#include "Config/AppConfig.h"
#include "KatianaSPM.h"

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/Board/Board.h>
#include <LUFA/Platform/Platform.h>

/* Preprocessor Checks: */
#if !defined(__OPTIMIZE_SIZE__)
#error This bootloader requires that it be optimized for size, not speed, to fit into the target device. Change optimization settings and try again.
#endif

/* Macros: */
#define BOOTLOADER_VERSION_MAJOR     0x02
#define BOOTLOADER_VERSION_MINOR     0x00
#define BOOTLOADER_HWVERSION_MAJOR   0x02
#define BOOTLOADER_HWVERSION_MINOR   0x01

/** 
Eight character bootloader firmware identifier reported to the host when requested. 
*/
#define SOFTWARE_IDENTIFIER          "LUFACDC"

/** 
This is the value stored at (FLASHEND-1) which is examined by Arduino core libraries (CDC.cpp)
to determine whether the boot key belongs at (RAMEND-1) or not.

- 0xDC42 -- boot key is at (RAMEND-1)
- Any other value -- boot key is at 0x0800

Placing the boot key at 0x0800 in SRAM was a low-risk bug. 
On rare occasions the Arduino sketch might be storing data at this location, and if it
just happend to contain the boot key value (0x7777), then a sketch-generated WDT reset
would not behave as expected. It would not totally break anything, but there would be 
an eight second delay on the reset during which the bootloader would appear on it's 
own CDC serial port.

We use (RAMEND-1) for the boot key so this must have this value or Arduino sketch uploads 
will not work.
*/
#define MAGIC_BOOT_KEY               0xDC42

/* Enums: */
/** Possible memory types that can be addressed via the bootloader. */
enum AVR109_Memories
{
    MEMORY_TYPE_FLASH  = 'F',
    MEMORY_TYPE_EEPROM = 'E',
};

/** Possible commands that can be issued to the bootloader. */
enum AVR109_Commands
{
    AVR109_COMMAND_Sync                     = 27,
    AVR109_COMMAND_ReadEEPROM               = 'd',
    AVR109_COMMAND_WriteEEPROM              = 'D',
    AVR109_COMMAND_ReadFLASHWord            = 'R',
    AVR109_COMMAND_WriteFlashPage           = 'm',
    AVR109_COMMAND_FillFlashPageWordLow     = 'c',
    AVR109_COMMAND_FillFlashPageWordHigh    = 'C',
    AVR109_COMMAND_GetBlockWriteSupport     = 'b',
    AVR109_COMMAND_BlockWrite               = 'B',
    AVR109_COMMAND_BlockRead                = 'g',
    AVR109_COMMAND_ReadExtendedFuses        = 'Q',
    AVR109_COMMAND_ReadHighFuses            = 'N',
    AVR109_COMMAND_ReadLowFuses             = 'F',
    AVR109_COMMAND_ReadLockbits             = 'r',
    AVR109_COMMAND_WriteLockbits            = 'l',
    AVR109_COMMAND_EraseFLASH               = 'e',
    AVR109_COMMAND_ReadSignature            = 's',
    AVR109_COMMAND_ReadBootloaderSWVersion  = 'V',
    AVR109_COMMAND_ReadBootloaderHWVersion  = 'v',
    AVR109_COMMAND_ReadBootloaderIdentifier = 'S',
    AVR109_COMMAND_ReadBootloaderInterface  = 'p',
    AVR109_COMMAND_SetCurrentAddress        = 'A',
    AVR109_COMMAND_ReadAutoAddressIncrement = 'a',
    AVR109_COMMAND_ReadPartCode             = 't',
    AVR109_COMMAND_EnterProgrammingMode     = 'P',
    AVR109_COMMAND_LeaveProgrammingMode     = 'L',
    AVR109_COMMAND_SelectDeviceType         = 'T',
    AVR109_COMMAND_SetLED                   = 'x',
    AVR109_COMMAND_ClearLED                 = 'y',
    AVR109_COMMAND_ExitBootloader           = 'E',
};

/* Bit mask to extract only the page start address from a flash address */
#define SPM_PAGEMASK (~(SPM_PAGESIZE-1))


/** 
Determine the proper setting for system clock prescaler,
or detect if parameters (F_USB and F_CPU) are invalid
*/
#if   F_USB == (F_CPU << 8)
    #define SYSTEM_CLOCK_PRESCALE clock_div_256
#elif F_USB == (F_CPU << 7)
    #define SYSTEM_CLOCK_PRESCALE clock_div_128
#elif F_USB == (F_CPU << 6)
    #define SYSTEM_CLOCK_PRESCALE clock_div_64
#elif F_USB == (F_CPU << 5)
    #define SYSTEM_CLOCK_PRESCALE clock_div_32
#elif F_USB == (F_CPU << 4)
    #define SYSTEM_CLOCK_PRESCALE clock_div_16
#elif F_USB == (F_CPU << 3)
    #define SYSTEM_CLOCK_PRESCALE clock_div_8
#elif F_USB == (F_CPU << 2)
    #define SYSTEM_CLOCK_PRESCALE clock_div_4
#elif F_USB == (F_CPU << 1)
    #define SYSTEM_CLOCK_PRESCALE clock_div_2
#elif F_USB == F_CPU
    #define SYSTEM_CLOCK_PRESCALE clock_div_1
#else
    #error Invalid clock frequencies: (F_USB / F_CPU) must be an integer power of two betwen 1 and 256.
#endif

/**
======================== Function Declarations ======================

We declare every function static if possible to make compiler optimizations
work better. This may in fact not help but it won't hurt either.

*/

static void inline SetupMinimalHardware(void);
static void inline SetupNormalHardware(void);

/** 
  Declaring this with the noreturn attribute saves a few bytes of flash.
  It does generate a compiler warning as the function actually WOULD return
  if the jmp 0x0000 weren't there -- and the compiler doesn't know that.
  Some clever person could maybe find a way to prevent that warning from 
  being emitted?
*/
static void __attribute__((noreturn naked noinline)) StartSketch( void );

#if defined(ENABLE_SECURITY_CHECKS)
    static uint8_t ValidateFlashBlock(uint16_t);
    static uint8_t ValidateEepromBlock(uint16_t);
#else
    #define ValidateFlashBlock(x) 1
    #define ValidateEepromBlock(x) 1
#endif

/** Top level I/O functions to send and receive bytes from the CDC COM port */
static uint8_t CdcReceiveByte(void);
static uint8_t       CdcFlush(void);
static void       CdcSendByte(const uint8_t Response);

#if defined(ENABLE_BLOCK_SUPPORT)
static void ReadWriteMemoryBlock(const uint8_t Command);
#endif

static void __attribute__((noinline)) SetTimeout(uint16_t);
static void __attribute__((noinline)) BootRwwEnable(void);
static void __attribute__((noinline)) ExecuteSPM( uint8_t );
 __attribute__((noinline)) static void IncrementAddress(void);
static void  __attribute__((noinline)) WriteProgmemArray(const uint8_t*, uint8_t);
static void __attribute__((noinline)) flashLed(void);

/** 
Function which grabs at most one AVR-910 command and fully processes it
for every invocation.
*/
static void ProcessAVR910Command(void);

/** 
Callbacks for events received by the LUFA USB drivers
These are referenced from the LUFA driver and cannot be static
*/
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);
