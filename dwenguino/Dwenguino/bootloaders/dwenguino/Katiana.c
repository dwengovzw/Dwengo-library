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
Copyright © 2017, aweatherguy
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

/*
The original version of this bootloader was named BootloaderCDC.c and was 
part of the LUFA-170418 distribution. That file has been extensively modified
into this: a bootloader compatible with one or more Arduino boards using MCUs
which have built-in USB interfaces. Examples are the Leonardo and LilyPad USB.
*/

/** 
\file
Most of the source code for the Katiana bootloader is in this file. 

This file has to antecedants:

- BootloaderCDC.c as distributed with LUFA-170418 (<tt>Bootloaders/CDC</tt>)
- Caterina.c as distributed with Arduino IDE 1.8.4 (<tt>hardware/arduino/avr/bootloaders/caterina-LilyPadUSB</tt>)

The original Caterina.c was written for LUFA-111009 and will not build with LUFA-170418.
This file is a result of modifying BootloaderCDC.c to implement the concepts
in Caterina.c. Bugs fixes and improvements have been included as well.
The result is a bootloader compatible with one or more Arduino boards using MCUs
which have built-in USB interfaces. Examples are the Leonardo and LilyPad USB.

Many global SRAM variables are not initialized. 
This saves a (very small) amount of time at startup. The only excections are:

- cdcPortSettings: by zeroing this we save a couple bytes of code space
over specifying all of the pertinent fields in an explicit initialization.

Everything possible is declared static to assist the compiler in optimizations.
This seems to save a significant amount of program code space (100 bytes or so).
It's not clear which static definitions are actually responsible for space reduction;
this is the shtogun approach.

The attributes on <tt>SaveCriticalInfo()</tt> and <tt>main()</tt> cause doxygen
to omit the functions from documentation so those are removed for doxygen builds.
We tried moving the attribute declarations to the header file as declarations only
and that doesn't seem to work.

<h3>Doxygen Issues</h3>

The usual attribute declarations cause doxygen to barf and lots of stuff
declared with attributes will be omitted from the output.
That's the purpose of LUFA's <tt>Attributes.h</tt> include file -- it contains
macros for various attributes; using the macros instead does not cause problems
with doxygen.
*/

#define INCLUDE_FROM_KATIANA_C
#include "Katiana.h"

/** 
Pre-assigned AVR910 MCU signatures are defined in Descriptors.h 
*/
static uint8_t avrSignature[3] PROGMEM = { AVR_SIGNATURE_3, AVR_SIGNATURE_2, AVR_SIGNATURE_1 };

/** 
String used to identify this bootloader to the AVR910 programmer.
\see Defined in Katiana.h
*/
static uint8_t softwareIdentifier[7] PROGMEM = SOFTWARE_IDENTIFIER;

/** 
Contains the current baud rate and other settings of the first virtual serial port. 
This must be initialized with valid data, as some operating systems will not open the 
port unless the initial settings are valid. See comments in main() for more on this.

This variable IS initialized to zeros, and we depend on that in the code.
*/
static CDC_LineEncoding_t cdcPortSettings;

/** 
<h3>Address Counters</h3>

We store the value sent to us from the host with the AVR910 Set Address command here. 
For flash this is a word address and for EEPROM it is a byte address.
With flash, this can address up to 64k words or 128kB of flash.
With EEPROM, we can address up to 64kB of EEPROM.

These values are auto-incremented during read and write operations.

When programming flash, we must double the AVR910 address to get the byte address in flash.
For MCUs with more than 64kB of flash, the byte address will overflow when 
the word address >= 0x8000. This overflow is okay because we keep the RAMPZ register correctly 
set so that loading the byte address into the Z register yields the correct result.
See the IncrementAddress() function for details.

When programming EEPROM, the AVR910 address is the byte address into EEPROM and 
we the flashByteAddress variable is a don't care.

\see flashByteAddress
\see IncrementAddress
*/
static uint16_t avr910Address ATTR_NO_INIT;
/** 
This is only used during flash read/write, not for EEPROM operations.
It is however kept in sync with avr910Address at all times.

\see avr910Address
*/
static uint16_t flashByteAddress ATTR_NO_INIT;
#if !defined(__DOXYGEN__)
#define avr910AddressLsb (*(uint8_t *)&avr910Address)
#define avr910AddressMsb (* (((uint8_t *)&avr910Address) + 1))
#endif

#if defined(ENABLE_FLASH_BYTE_SUPPORT) && !defined(__DOXYGEN__)
/**
If flash byte support is enabled, we use a 2-byte buffer to assemble two bytes into a word
for programming to flash. There is also a cast defined to access this buffer as an unsigned word.
*/
static uint8_t flashByteBuffer[2] ATTR_NO_INIT;
#define flashWordBuffer (uint16_t *)flashByteBuffer;
#endif

/** 
<h3>Boot Key</h3>
Newer version of Arduino cores will pass a special "boot key" to the boot loader in a pre-arranged
location in SRAM. That location was originally 0x0800, but has since been moved to (RAMEND-1) to 
avoid the possibility of a sketch storing data at 0x0800. If the sketch happened to store the value
0x7fff there (very remote possibility), then the boot behavior resulting from a sketch-induced WDT
reset would change unexpectedly.

When this code starts up, it saves the value stored at the boot key location in a global variable.
*/
#define bootKey (*(volatile uint16_t *)(RAMEND-1))
/** 
The value of the boot key at reset time is saved here by a function in the .init0 section.
*/
static uint16_t originalBootKey ATTR_NO_INIT;
#if !defined(__DOXYGEN__)
#define BOOT_KEY 0x7777
#endif

/**
Sometimes the sketch would like to know the reason for the last MCU reset when it starts.
We save a copy of MCUSR at the beginning so we can pass it to the sketch.
*/
static uint8_t initialMCUSR ATTR_NO_INIT;

/**
This is set based on whether the reset vector at location zero is all ones or not.
if a sketch is loaded, the reset vector will not be all ones.
*/
static volatile uint8_t sketchPresent;

/**
timeout is decremented in the Timer 1 compare match ISR and stops when it reaches zero.
This is used to time certain behaviors in the bootloader.
timeout ticks are  at 25Hz, 40ms per tick. 
8 seconds = 200 ticks, 760ms = 19 ticks, etc
*/
static volatile uint8_t timeout ATTR_NO_INIT;

#if !defined(__DOXYGEN__)
#define TIMEOUT_PERIOD			    200
#define SHORT_TIMEOUT_PERIOD		 12
#define EXT_RESET_TIMEOUT_PERIOD	 19
#endif

#if defined(LED_DATA_FLASHES)  || defined(__DOXYGEN__)
/**
<h3>LED Data Flashing</h3>
Whenever the TX/RX LEDs flash, they will go on for one timer tick and then off
for one timer tick. The on-then-off sequence is called a "flash cycle" and 
does not end until the LED has been off for one timer tick (1/25 of a second).

These two variables are request flags to the timer match ISR
to begin a flash cycle. They only have two states -- zero and non-zero.

To request a flash, just set one of them non-zero (0xff recommended).

If a flash cycle is already in progress, these variables are not examined
until the cycle is complete. When a flash cycle is completed, if these variables
are non-zero, the ISR will begin another flash cycle for that LED, 
and zero the request flag.

If multiple flash cycles are requested during a flash cycle, only one flash
cycle will result. That's the desired behavior.

\see ISR
\see rxLedFlash
*/
static volatile uint8_t txLedFlash;
/**
\see txLedFlash
*/
static volatile uint8_t rxLedFlash;
#endif

#if (LED_START_FLASHES > 0) || defined(__DOXYGEN__)
/**
When LED start flashes are enabled, this is used in the timer compare match ISR to
count the number of on plus off periods generated. 
Since the compare match rate (25Hz) is too fast for these flashes, it is divided by
four to generate toggles of the LED state.
It must be initialized to a multiple of 8 plus 1 if the LED is to finish in the OFF state.
*/
static volatile uint8_t ledFlashCount; // ATTR_NO_INIT;
#endif

/** 
We need a small function in the to make copies of important data before it
gets overwritten by the initialization code:

- A copy of the boot key passed from the sketch, and
- The value of MCUSR which contains the cause of the reset which has started the boot loader.
While MCUSR may not get altered by startup code, this is extra insurance.
*/
void 
#if !defined(__DOXYGEN__)   // doxygen barfs on these attribute declarations. bad dog. no biscuit.
    __attribute__ ((naked)) 
    __attribute__ ((section (".init0"))) 
    __attribute__ ((used))	    // makes sure the compiler won't strip this function
#endif
    SaveCriticalInfo(void)
{
    initialMCUSR    = MCUSR;
    originalBootKey = bootKey;
}

static void SetTimeout(uint8_t howLong)
{
    cli();
    timeout = howLong;
    sei();
}

/** 
Performs a minimal amount of cleanup and then jumps to the reset vector
(at location zero) to start the arduino sketch.
It passes the initial value in MCUSR to the sketch in register r2.
Do NOT call this function if there is no sketch loaded!!!

Several attributes are applied to this function in Katiana.h:

- noreturn: causes the compiler to use rjmp instead of rcall, saving 2 bytes per call.
- naked:    ensures that there will be no ret instruction at the end.
- noinline: this is called from several places and cannot be inlined.

There WILL be compiler warning about the fuction actually returning.
Not sure how to get rid of that, but it is does not reflect any bugs in the resulting code.
*/
static void StartSketch( void )
{
    cli();

    /* Undo TIMER1 setup and clear the count before running the sketch */
    TIMSK1 = 0;
    TCCR1B = 0;

    /* Put DDR and PORT registers back to their original state */
    LEDs_Disable();

    /* Relocate the interrupt vector table to the application section */
    MCUCR = (1 << IVCE);
    MCUCR = 0;
    // 
    // Save the reset flags in a CPU register (R2) for the sketch to recover.
    // This can be accessed in the sketch by putting code in .init0, which executes before normal c init code.
    // That code can save r2 in a global variable for later use. See comments at top of this file
    // for a code example showing how to do this.
    //
    __asm__ __volatile__ ("mov r2, %0\n" : : "r" (initialMCUSR) );
    //
    // here are two ways to jump to the reset vector:
    //
    // 1) the goto uses 6 bytes -- two LDI and one IJMP instruction.
    //    that's two more bytes than the 4-byte "jmp 0x0000" instruction.
    //    This method gets rid of a compiler warning that this no-return function actually does return.
    //
    // 2) If you really need the extra two bytes, then use the "jmp 0x0000" instead and live
    //    with the compiler warning.
    //
    
    goto *0; // maybe only works with GCC based compilers.
    
    // __asm__ __volatile__ ( "jmp 0x0000\n" : : );
}

/** 
This function implements Arduino-specific reset/boot behavior.
Immediately on a reset, we save the current value at the boot key memory location and then zero it.
The ONLY way the original boot key will be equal to BOOT_KEY is if the sketch modified
it to be so. If the sketch does this, the following behaviors in the boot loader will change:

1) On an external reset, the bootloader will pause for 750ms then jump into the sketch.
This provides a window in which the user can generate a 2nd external reset, and this
will cause the bootloader to run with an 8 second timeout.

2) If reset was due to WDT, the sketch will be restarted immediately with no delay.
This is how the Arduino core causes a reboot when the CDC COM port is touched at 1200 baud.

Do NOT call this function if there is no sketch present!!!
*/
static void inline SketchStartLogic(void)
{
    //
    // Power-on and brown-out resets will always cause the sketch to load immediately.
    //
    uint8_t pwrReset = initialMCUSR & (_BV(PORF) | _BV(BORF));

    if (pwrReset)
    {	
        StartSketch();
    } 
    //
    // if the boot key is active, the don't run the sketch...
    //
    uint8_t booty = originalBootKey - BOOT_KEY;
    if (! booty) return;
    //
    // here, the boot cause is one of the following: External, WDT, JTAG, USB.
    // special behaviors only occur with external and WDT resets. All others
    // will fail the following chesks and return to run the bootloader.
    //
    if ( initialMCUSR & _BV(EXTRF) )
    {
        // make the boot key active so that another external reset within the 750ms
        // window will cause the boot loader to run instead of winding up here again.
        bootKey = BOOT_KEY; 
        //
        // setup a 750ms timeout
        //
        SetTimeout( EXT_RESET_TIMEOUT_PERIOD ); // this will enable interrupts too
        //
        // This is a 750ms window in which the user can generate an external reset.
        // That will force the boot loader to run, instead of us starting the sketch
        // If the 750ms period expires with no external reset, the bootloader will run
        //
        while (timeout);
        //
        // timeout expired without another external reset, so we can load the sketch now.
        // init code for the sketch will overwrite the bootKey with a return address,
        // so we don't need to inactivate it here...
        //
        // bootKey = 0;			// set the bootKey back to inactive. 
        StartSketch();
    } 
    //
    // On a watchdog reset, if the BOOT_KEY is inactive and there's a sketch, we go
    // straight to the sketch. This allows the sketch to purposely generate a WDT reset
    // and immediately jump back into the sketch.
    //
    // If things get hosed in the sketch to where the boot key isn't being set active on
    // a reboot cause by touching the CDC interface at 1200 baud, it will be necessary to use
    // a double-tap on the external reset to get into the bootloader.
    //
    if ( initialMCUSR & _BV(WDRF) )
    {
        StartSketch();
    }
}

/**
Calls boot_rww_enable() with interrupts disabled, then re-enables them.
*/
static void BootRwwEnable( void )
{
    cli();
    boot_rww_enable();
    sei();
}

/** 
main() performs these tasks:
- Initialize hardware
- Check for loaded sketch and jump to it under Arduino-specific conditions
- Run the bootloader
- Start sketch if requested by bootloader

The actual signature of this function is as follows:

\code
__attribute__ ((OS_main)) __attribute__ ((section (".init9"))) main() { ... }
\endcode

It is in section .init9; this removes the interrupt vector table
we don't need. It is also 'OS_main', which means the compiler does not
generate any entry or exit code itself (but unlike 'naked', it doesn't
supress some compile-time options we want.)
This idea was borrowed from the Optiboot project.

main() is referenced from startup code and therefore cannot be static.
*/
int
#if !defined(__DOXYGEN__)   // doxygen barfs on these attribute declarations! bad dog. no biscuit.
__attribute__ ((OS_main)) 
__attribute__ ((section (".init9")))
#endif
main(void)
{
    //
    // Setup minimal amount of hardware required to get started.
    //
    SetupMinimalHardware();

    bootKey = 0;
    //
    // if progmem == 0xffff, adding one rolls over to 0x0000
    //
    if  (pgm_read_word_near(0) != 0xFFFFu)
    {
        sketchPresent = 0xffu;
        SketchStartLogic();
    }

    initialMCUSR |= 0x80u;	// flag the fact that the boot loader did not immediately start the sketch.

#if (LED_START_FLASHES > 0)
    //
    // Timer 1 runs at 25Hz; the ISR will toggle the led at 25Hz/4 which 
    // gives a flash rate of 25Hz/8 or about 3Hz
    //
    ledFlashCount = (LED_START_FLASHES << 2) + 1;
#endif
    //
    // We're going to run the bootloader and that requires a bit more hardware initialization
    //
    SetupNormalHardware();

    SetTimeout( TIMEOUT_PERIOD );
    //
    // timeout only decrements when there's a sketch loaded, so this loop
    // runs forever until someone loads page address zero
    //
    while (timeout)
    {
        ProcessAVR910Command();
        //
        // Note: you can save two bytes by copying the contents of USB_DeviceTask()
        // here instead of calling it...See USBTask.c in LUFA/Drivers/USB/Core.
        //
        USB_USBTask();
        //
        // The command processor may leave one of the LEDs on. Make sure
        // they are both off at the end of each pass through this loop.
        //
        LEDs_RX_Off();
        LEDs_TX_Off();
    }

    /* Wait a short time to wrap up all USB transactions and then disconnect */
    _delay_us(1000);

    /* Disconnect from the host - USB interface will be reset later along with the AVR */
    USB_Detach();

    StartSketch();
}

/** 
Configures minimum amount of hardware to get started with.
More setup will be required of the bootloader needs to be run
(instead of jumping right into the sketch).
*/
static void SetupMinimalHardware(void)
{
    MCUSR = 0;
    //
    // wdt_disable() has too much "stuff" for our needs --
    // it disables interrupts and saves/restores SREG...
    // so we do it the direct way here, saving six bytes
    //
    WDTCSR = _BV(WDE) | _BV(WDCE);
    WDTCSR = 0x00;
    //
    // Setup the proper MCU clock rate (defined in Katiana.h)
    // ditto about the AVR macro...we do it the direct way here, saving eight bytes
    //clock_prescale_set( SYSTEM_CLOCK_PRESCALE );
    //
    CLKPR = _BV(CLKPCE);
    CLKPR = SYSTEM_CLOCK_PRESCALE;
}

/**
Configures additional hardware that's required to run the bootloader.
SetupMinimalHardware() must be called before this function.
*/
static void SetupNormalHardware()
{
    // Relocate the interrupt vector table to the bootloader section
    MCUCR = (1 << IVCE);
    MCUCR = (1 << IVSEL);
    /*
    Not sure if this is necessary? 
    Does the USB host always set line encoding before reading it? If so, this is not required.
    Being static, cdcPortSettings has already been zeroed during init and that is the proper init
    value for all fields except for DataBits. By setting only DataBits here compared to doing
    it in the declaration saves two bytes of flash.
    */
    cdcPortSettings.DataBits = 8;
    USB_Init();

    LEDs_Init();
    //
    // Start timer 1 running in CTC mode with a 25Hz interrupt rate
    // It will count up to the value in OCR1A then reset back to zero again, generating
    // a compare match interrupt with every cycle.
    //
    OCR1A = TICK_COUNT_25HZ;		        // compare match value which generates 25Hz interrupt rate
    TIMSK1 = _BV(OCIE1A);		            // enable timer 1 output compare A match interrupt
    TCCR1B = TCCR1B_CS_25HZ | _BV(WGM12);   // setup the prescaler and CTC mode.
    // TCNT1 = 0;			                // After MCU reset, TCNT1 is cleared; no need do that here.

}

/** 
Timer 1 is configured to fire this compare match ISR at a 25Hz rate (40ms per compare match).
This ISR provides a timeout function in addition to flashing LEDs as needed.

Given the 40ms timer period, 
it is possible to generate timeouts up to 10 seconds with only 8-bit timeout counters.

The "L" LED is simply flashed a fixed number of times at startup to indicate
there's some form of life in the bootloader.

The TX and RX LEDs are flashed when programming data is transferred to or from 
the bootloader.
See the definition of txLedFlash for more information on how flash cycles work for the
TX and RX LEDs.

\see timeout
\see ledFlashCount 
\see rxLedFlash 
\see txLedFlash
*/
ISR(TIMER1_COMPA_vect, ISR_BLOCK)
{
    if ( sketchPresent & timeout ) timeout--;

#if defined( LED_DATA_FLASHES )
    if (LEDS_RX_TEST)
    {
        LEDs_RX_Off();
    }
    else
    {
        if (rxLedFlash)
        {
            LEDs_RX_On();
            rxLedFlash = 0;
        }
    }

    if (LEDS_TX_TEST)
    {
        LEDs_TX_Off();
    }
    else
    {
        if (txLedFlash)
        {
            LEDs_TX_On();
            txLedFlash = 0;
        }
    }
#endif

#if (LED_START_FLASHES > 0)
    if (ledFlashCount)
    {	
        ledFlashCount--;
        //
        // toggle LED on every 4th timer tick, which effectively divides
        // the 25Hz timer rate by eight, for a 3.125Hz flash rate.
        //
        if ((ledFlashCount & 0x03) == 0)
        {
            LEDs_L_Toggle();
        }
    }
#endif
}

/** 
Event handler for the USB_ConfigurationChanged event. This configures the device's endpoints ready
to relay data to and from the attached USB host.
*/
void EVENT_USB_Device_ConfigurationChanged(void)
{
    /* Setup notifications for CDC, Rx and Tx Endpoints */
    Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT, CDC_NOTIFICATION_EPSIZE, 1);

    Endpoint_ConfigureEndpoint(CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);

    Endpoint_ConfigureEndpoint(CDC_RX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);
}

/** 
Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
the device from the USB host before passing along unhandled control requests to the library for processing
internally.
*/
void EVENT_USB_Device_ControlRequest(void)
{
    if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
    {
        if (USB_ControlRequest.bRequest == CDC_REQ_SetLineEncoding)
        {
            Endpoint_ClearSETUP();

            /* Read the line coding data in from the host into the global struct */
            Endpoint_Read_Control_Stream_LE(&cdcPortSettings, sizeof(CDC_LineEncoding_t));
            Endpoint_ClearIN();
        }
        else if (USB_ControlRequest.bRequest == CDC_REQ_SetControlLineState)
        {
            Endpoint_ClearSETUP();
            Endpoint_ClearStatusStage();
        }
    }
    else if ( (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE)) &&
              (USB_ControlRequest.bRequest == CDC_REQ_GetLineEncoding) )
    {
            Endpoint_ClearSETUP();

            /* Write the line coding data to the control endpoint */
            Endpoint_Write_Control_Stream_LE(&cdcPortSettings, sizeof(CDC_LineEncoding_t));
            Endpoint_ClearOUT();
    }
}

/**
This function performs a safe SPM sequence, in the order listed below.
It cannot be used for the boot page fill operation.

- Disable global interrupts
- Write SpmcsrValue so SPMCSR/SPMCR
- Execute SPM instruction
- Enable global interrupts
- Busy-wait for the SPM operation to complete

\param[in] SpmcsrValue is the value to be loaded into the SPMCSR/SPMCR register.
*/
static void ExecuteSPM( uint8_t SpmcsrValue )
{
    cli();
    spm( SpmcsrValue );	// defined in KatianaSPM.h
    sei();
    boot_spm_busy_wait();
}

/**
Increments the AVR910 address by one, Avr910ByteAddress by two,
and updates RAMPZ if the byte address overflows.

\see Side effects: avr910Address, flashByteAddress; RAMPZ may be modified.
*/
static void IncrementAddress(void)
{
    avr910Address++;
    flashByteAddress = avr910Address << 1; // += 2; Left shift is 8 bytes less code space
#if defined(RAMPZ)
    //
    // if byte address rolled over to 0 or 1, then set the RAMPZ lsb.
    // this makes things work correctly for parts with 128kB of flash.
    //
    if ((flashByteAddress & 0xFFFEu) == 0) RAMPZ = 0x01;
#endif
}

/** 
Some AVR-910 command responses are kept only in flash and this function
copies them out to the USB host using CdcSendByte().

\param[in] Response is an array of bytes in PROGMEM to be sent to the USB host.
\param[in] Count is the number of bytes to be sent.
\see CdcSendByte
*/
static void WriteProgmemArray(uint8_t *Response, uint8_t Count)
{
    while (Count--) CdcSendByte( pgm_read_byte_near( Response++ ) );
}

#if defined(ENABLE_SECURITY_CHECKS) || defined(__DOXYGEN__)

/** 
Checks whether the flash memory block running from avr910Address to 
(avr910Address + (BlockSize >> 1)) represents a valid section of flash memory.

\param[in] BlockSize is the number of bytes in the proposed data block to be checked.
\return Returns non-zero if the block is valid.
\see Side effects: [in] avr910Address
*/
static uint8_t ValidateFlashBlock(uint16_t BlockSize)
{
    //
    // ================ WARNING WARNING WARNING =====================
    // SPM_PAGESIZE is BYTES -- not words as Atmel some datasheets might have you beleive
    //
    if (BlockSize > (uint16_t)(SPM_PAGESIZE)) return 0;
    uint16_t blockWords = (BlockSize + 1u) >> 1;
    uint16_t endWordAddr = avr910Address + blockWords - 1u;
    if (endWordAddr < avr910Address) return 0;    // overflow not allowed
    //
    // the operation is not allowed to span more than a single flash page
    //
    if ((avr910Address & SPM_PAGEMASK) != (endWordAddr & SPM_PAGEMASK)) return 0;
    //
    // the operation may not include any bootloader flash space
    //
    if (    endWordAddr >= (BOOT_START_ADDR >> 1)) return 0;

    return 0xff;
}

/** 
Checks whether the EEPROM memory block running from avr910Address to (avr910Address + (BlockSize >> 1))
represents a valid section of EEPROM memory. We have no restrictions on block size here because
each EEPROM byte must be individually read/written. In this situation, avr910Address is
the BYTE address in EEPROM to be checked. BlockSize is still in bytes.

\param[in] BlockSize is the number of bytes in the proposed data block to be checked.
\return Returns non-zero if the block is valid.
\see Side effects: [in] avr910Address
*/
static uint8_t ValidateEepromBlock(uint16_t BlockSize)
{
    if (BlockSize > (E2END + 1u)) return 0;
    if (avr910Address > E2END) return 0;
    uint16_t endAddr = avr910Address + BlockSize - 1u;
    if (endAddr < avr910Address) return 0; // overflow not allowed.
    if (endAddr > E2END) return 0;
    return 0xff;
}
#endif

#if defined(ENABLE_BLOCK_SUPPORT) || defined(__DOXYGEN__)
/** 
Reads or writes a block of EEPROM or FLASH memory to or from the appropriate CDC data endpoint, depending
on the AVR109 protocol command issued.

\param[in] Command  Single character AVR109 protocol command indicating what memory operation to perform
\see Side effects: [in,out] avr910Address, [in,out] flashByteAddress, [in,out] <tt>RAMPZ</tt>.
*/
static void ReadWriteMemoryBlock(const uint8_t Command)
{
    uint16_t word;
    uint16_t blockSize;
    char MemoryType;
    uint8_t isFlash;
    uint8_t isEeprom;
    //
    // Getting two bytes from the CDC serial port into a 16-bit integer
    // is more efficient (4 bytes less code) if we do it in assembly.
    // Be careful to build the 16-bit value in a lower register 
    // so the 2nd call to CdcReceiveByte doesn't over-write the MSB.
    //
    __asm__ __volatile__ (
        "rcall CdcReceiveByte\n\t"
        "mov %B0, r24\n\t"
        "rcall CdcReceiveByte\n\t"
        "mov %A0, r24\n"
        : "=l" (blockSize)
        : 
        : "r24","r25"
    );

    MemoryType =  CdcReceiveByte();
    isFlash  = MemoryType == MEMORY_TYPE_FLASH;
    isEeprom = MemoryType == MEMORY_TYPE_EEPROM;

    if ( (! isFlash) && (! isEeprom) )
    {
        /* Send error byte back to the host */
        CdcSendByte('?');
        return;
    }

    uint8_t valid = isFlash ? ValidateFlashBlock(blockSize) : ValidateEepromBlock(blockSize);
    uint16_t blockStartAddress = flashByteAddress;
    uint8_t isWrite = Command == AVR109_COMMAND_BlockWrite;

    uint8_t writingFlash = valid & isWrite & isFlash;
    if (writingFlash)
    {
        __asm__ __volatile__ (		    \
            "movw r30,%0\n"			    \
            :				    \
            : "r"((uint16_t)blockStartAddress)   \
            );
        ExecuteSPM( __BOOT_PAGE_ERASE );
    }

    while (blockSize--)
    {
        if (isWrite)
        {
            if (isFlash)
            {
                //
                // Getting two bytes from the CDC serial port into a 16-bit integer 
                // is more efficient if we do it in assembly. 
                // The entire change in this case saves a total of 26 bytes compared to previous
                // method involving a 2-byte array to hold the prog memory word data.
                // Be careful to build the 16-bit value in a lower register 
                // so the 2nd call to CdcReceiveByte doesn't over-write the MSB.
                //
                __asm__ __volatile__ (
                    "rcall CdcReceiveByte\n\t"
                    "mov %A0, r24\n\t"
                    "rcall CdcReceiveByte\n\t"
                    "mov %B0, r24\n"
                    : "=l" (word)
                    : 
                    : "r24","r25"
                );

                if (valid)
                {
                    // 
                    // boot_page_fill automatically enables the RWW section, so we don't
                    // need to clear that state from the page erase operation.
                    //
                    cli();
                    boot_page_fill(flashByteAddress, word); //*(uint16_t*)word);
                    sei();
                }

                blockSize--;
            }
            else
            {
                if (valid)
                {
                    cli();
                    eeprom_update_byte((uint8_t*)((intptr_t)avr910Address), CdcReceiveByte());
                    sei();
                }
            }
        }
        else
        {
            if (isFlash)
            {
                uint16_t w;
                w = 0xffffu;
                if (valid) w = pgm_read_word( flashByteAddress );
                CdcSendByte(w & 0xffu);
                CdcSendByte(w >> 8);
                blockSize--;
            }
            else
            {
                CdcSendByte(valid ? eeprom_read_byte((uint8_t*)(intptr_t)avr910Address) : 0xffu);
            }
        }
        IncrementAddress();
    }

    if (writingFlash)
    {
        ExecuteSPM( __BOOT_PAGE_WRITE );
        BootRwwEnable();
        if (blockStartAddress == 0) sketchPresent = 0xffu;
    }

    eeprom_busy_wait(); // shouldn't hurt if we didn't write to eeprom...

    if ( isWrite ) CdcSendByte('\r');

    //
    // every time we receive flash read/write block command, restart the 8-second timeout period.
    //
    SetTimeout( TIMEOUT_PERIOD );
}

#endif

/** 
Retrieves the next byte from the host in the CDC data OUT endpoint, and clears the endpoint bank if needed
to allow reception of the next data packet from the host.

\return Next received byte from the host in the CDC data OUT endpoint
*/
static uint8_t __attribute__((noinline)) CdcReceiveByte(void)
{
    /* Select the OUT endpoint so that the next data byte can be read */
    Endpoint_SelectEndpoint(CDC_RX_EPADDR);

    /* If OUT endpoint empty, clear it and wait for the next packet from the host */
    while (!(Endpoint_IsReadWriteAllowed()))
    {
        Endpoint_ClearOUT();

        while (!(Endpoint_IsOUTReceived()))
        {
            if (USB_DeviceState == DEVICE_STATE_Unattached)
                return 0;
        }
    }

#ifdef LED_DATA_FLASHES
    rxLedFlash = 0xff;  // request a flash
#endif

    /* Fetch the next byte from the OUT endpoint */
    return Endpoint_Read_8();
}

/** 
Writes a byte to the CDC data IN endpoint, and sends the endpoint back if needed to free up the
bank when full ready for the next byte in the packet to the host.

\returns Zero on success, Non-zero if the USB interface was detached.
*/
static uint8_t __attribute__((noinline)) CdcFlush()
{
    Endpoint_ClearIN();

    while (!Endpoint_IsINReady())
    {
        if (USB_DeviceState == DEVICE_STATE_Unattached)
            return 0xff;
    }

    return 0;
}

static void __attribute__((noinline)) CdcSendByte(const uint8_t Data)
{
    /* Select the IN endpoint so that the next data byte can be written */
    Endpoint_SelectEndpoint(CDC_TX_EPADDR);

    /* If IN endpoint full, clear it and wait until ready for the next packet to the host */
    if (!(Endpoint_IsReadWriteAllowed()))
    {
        if (CdcFlush()) return;
    }

#ifdef LED_DATA_FLASHES
    txLedFlash = 0xff;  // request a flash
#endif

    /* Write the next byte to the IN endpoint */
    Endpoint_Write_8(Data);
}

/** 
Reads in a single AVR109 command from the CDC data OUT endpoint (if there's one available).
It will then process that command, and perform the required actions, sending the appropriate response back 
to the host.
*/
static void ProcessAVR910Command(void)
{
    /* Select the OUT endpoint */
    Endpoint_SelectEndpoint(CDC_RX_EPADDR);

    /* Check if endpoint has a command in it sent from the host */
    if (!(Endpoint_IsOUTReceived()))
        return;

    /* Read in the bootloader command (first byte sent from host) */
    uint8_t Command = CdcReceiveByte();

    if (Command == AVR109_COMMAND_ExitBootloader)
    {
        //
        // force the bootloader to stop accepting input in a short time (e.g. 500ms)
        // and jump into the sketch (if there is one).
        //
        SetTimeout( SHORT_TIMEOUT_PERIOD );

        /* Send confirmation byte back to the host */
        CdcSendByte('\r');
    }
    else if ((Command == AVR109_COMMAND_SetLED) || (Command == AVR109_COMMAND_ClearLED) ||
        (Command == AVR109_COMMAND_SelectDeviceType))
    {
        CdcReceiveByte();

        /* Send confirmation byte back to the host */
        CdcSendByte('\r');
    }
    else if ((Command == AVR109_COMMAND_EnterProgrammingMode) || (Command == AVR109_COMMAND_LeaveProgrammingMode))
    {
        /* Send confirmation byte back to the host */
        CdcSendByte('\r');
    }
    else if (Command == AVR109_COMMAND_ReadPartCode)
    {
        /* Return ATMEGA128 part code - this is only to allow AVRProg to use the bootloader */
        CdcSendByte(0x91);	// for ATmega32
        CdcSendByte(0x00);
    }
    else if (Command == AVR109_COMMAND_ReadAutoAddressIncrement)
    {
        /* Indicate auto-address increment is supported */
        CdcSendByte('Y');
    }
    else if (Command == AVR109_COMMAND_SetCurrentAddress)
    {
        //
        // Set the current address to that given by the host.
        //
        // ***** Interpretation of this address depends on the memory being referenced *****
        // ***** Flash:  this is a WORD address
        // ***** EEPROM: this is a BYTE address
        //
        // For flash programming, some special care is required with this address.
        // We keep two 16-bit versions of this address -- the original one plus (original << 1).
        // The original address works with flash sizes up to 128kB. 
        // So does the (original << 1), but with some caveats. 
        // We must set RAMPZ (if it exists) based on the msb of the original address,
        // and that takes care of overflows that occur if the word address >= 0x8000.
        //
        // There's one other place where this requires special consideration however, and that'
        // when auto-increment on the word address goes from 0x7fff to 0x8000 and the byte
        // address goes from 0xffff to 0x0000 -- when that happens we must set the lsb in RAMPZ.
        // In the case of some boot clients this issue may not come up -- they may send a set address
        // command prior to every block upload of a page.
        //
        uint8_t highByte = CdcReceiveByte();
        avr910AddressMsb = highByte;
        avr910AddressLsb = CdcReceiveByte();
#ifdef RAMPZ
        RAMPZ = (highByte & 0x80) ? 1 : 0;
#endif
        flashByteAddress = avr910Address << 1;
        /* Send confirmation byte back to the host */
        CdcSendByte('\r');
    }
    else if (Command == AVR109_COMMAND_ReadBootloaderInterface)
    {
        /* Indicate serial programmer back to the host */
        CdcSendByte('S');
    }
    else if (Command == AVR109_COMMAND_ReadBootloaderIdentifier)
    {
        /* Write the 7-byte software identifier to the endpoint */
        WriteProgmemArray( softwareIdentifier, 7 );
    }
    else if (Command == AVR109_COMMAND_ReadBootloaderSWVersion)
    {
        CdcSendByte('0' + BOOTLOADER_VERSION_MAJOR);
        CdcSendByte('0' + BOOTLOADER_VERSION_MINOR);
    }
    else if (Command == AVR109_COMMAND_ReadSignature)
    {
        WriteProgmemArray( avrSignature, 3 );
    }
#if defined(ENABLE_FLASH_ERASE_SUPPORT)
    else if (Command == AVR109_COMMAND_EraseFLASH)
    {
        //
        // Clear the application section of flash. The AVR910 command is actually supposed
        // to erase the entire chip ... ??? not sure if that's correct, but we only 
        // erase the flash in this bootloader.
        //
        // There are two versions of this -- one for parts with 64kB or less of flash, 
        // and another for parts with 128kB. We don't support more than 128kB of flash.
        //
#if (BOOT_START_ADDR < 0xFFFF)
		//for (uint16_t addr = 0; addr < (uint32_t)BOOT_START_ADDR; addr += SPM_PAGESIZE)
        //
        // decrement loops are faster, but we must make sure that addr will decrement exactly to zero.
        // We know that SPM_PAGESIZE is a power of 2. Anding with the pagemask should not be
        // necessary as BOOT_START_ADDR should be an integer multiple of SPM_PAGESIZE, but we
        // do it for safety. It's all just compile time math so no hit on flash usage.
        //
        uint16_t addr = (BOOT_START_ADDR & SPM_PAGEMASK);
        do 
		{
            addr -= SPM_PAGESIZE;
            cli();
		    boot_page_erase(addr);
            sei();
		    boot_spm_busy_wait();
		} while ( addr );
#else
        RAMPZ = 0;
        //for (uint16_t wordAddr = 0; wordAddr < (uint16_t)(BOOT_START_ADDR >> 1); wordAddr += (SPM_PAGESIZE >> 1))
        
        uint16_t wordAddr = (uint16_t)(BOOT_START_ADDR >> 1);
        do
        {
            wordAddr -= (SPM_PAGESIZE >> 1);
            if (wordAddr & 0x8000u) RAMPZ = 1;

            __asm__ __volatile__ (		    \
                "movw r30,%0\n"			    \
                :				    \
                : "r"(wordAddr << 1)   \
                );
            ExecuteSPM( __BOOT_PAGE_ERASE );
        } while ( addr );
#endif

        BootRwwEnable();

        /* Send confirmation byte back to the host */
        CdcSendByte('\r');
    }
#endif
#if defined(ENABLE_LOCK_BYTE_WRITE_SUPPORT)
    else if (Command == AVR109_COMMAND_WriteLockbits)
    {
        /* Set the lock bits to those given by the host */
        cli();
        boot_lock_bits_set(CdcReceiveByte());
        sei();

        /* Send confirmation byte back to the host */
        CdcSendByte('\r');
    }
#endif
#if defined(ENABLE_LOCK_FUSE_READ_SUPPORT)
    else if (Command == AVR109_COMMAND_ReadLockbits)
    {
        CdcSendByte(boot_lock_fuse_bits_get(GET_LOCK_BITS));
    }
    else if (Command == AVR109_COMMAND_ReadLowFuses)
    {
        CdcSendByte(boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS));
    }
    else if (Command == AVR109_COMMAND_ReadHighFuses)
    {
        CdcSendByte(boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS));
    }
    else if (Command == AVR109_COMMAND_ReadExtendedFuses)
    {
        CdcSendByte(boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS));
    }
#endif
#if defined(ENABLE_BLOCK_SUPPORT)
    else if (Command == AVR109_COMMAND_GetBlockWriteSupport)
    {
        CdcSendByte('Y');
        //
        // Send block size in BYTES to the host 
        // ================ WARNING WARNING WARNING =====================
        // SPM_PAGESIZE is BYTES -- not words as Atmel some datasheets might have you beleive
        //
        CdcSendByte(SPM_PAGESIZE >> 8);
        CdcSendByte((SPM_PAGESIZE) & 0xFF);
    }
    else if ((Command == AVR109_COMMAND_BlockWrite) || (Command == AVR109_COMMAND_BlockRead))
    {
        ReadWriteMemoryBlock(Command);
    }
#endif
#if defined(ENABLE_FLASH_BYTE_SUPPORT)
    //
    // It seems the way these commands must be used is to first set
    // current address to start of a page, then start issuing
    // FillFlashPageWordHigh then Low (in that order) until the
    // page is filled, then issue the WriteFlashPage command.
    // 
    // ===> These flash byte commands are NOT tested and may need some work <===
    //
    else if (Command == AVR109_COMMAND_FillFlashPageWordHigh)
    {
        /* Write the high byte to the current flash page */
        flashByteBuffer[1] = CdcReceiveByte();

        /* Send confirmation byte back to the host */
        CdcSendByte('\r');
    }
    else if (Command == AVR109_COMMAND_FillFlashPageWordLow)
    {
        uint16_t w;
        /* Write the low byte to the current flash page */
        flashByteBuffer[0] = CdcReceiveByte();
        w = *flashWordBuffer;

        if (ValidateFlashBlock(2))
        {
            cli();
            boot_page_fill( flashByteAddress, w );
            sei();
        }
        IncrementAddress();
        /* Send confirmation byte back to the host */
        CdcSendByte('\r');
    }
    else if (Command == AVR109_COMMAND_WriteFlashPage)
    {
        // this may be wrong too...what's the procedure w/no temp buffer???
        // what about security checks?
        /* Commit the flash page to memory */
        ExecuteSPM( __BOOT_PAGE_WRITE );
        BootRwwEnable();

        /* Send confirmation byte back to the host */
        CdcSendByte('\r');
    }
    else if (Command == AVR109_COMMAND_ReadFLASHWord)
    {	
        uint16_t ProgramWord = ValidateFlashBlock(2) ? pgm_read_word(flashByteAddress) : 0xffffu;

        CdcSendByte(ProgramWord >> 8);
        CdcSendByte(ProgramWord & 0xFF);
    }
#endif
#if defined(ENABLE_EEPROM_BYTE_SUPPORT)
    else if (Command == AVR109_COMMAND_WriteEEPROM)
    {
        /* Read the byte from the endpoint and write it to the EEPROM */
        uint8_t bite = CdcReceiveByte();

        if (ValidateEepromBlock(1))
        {
            cli();
            eeprom_update_byte((uint8_t*)((intptr_t)avr910Address), bite);
            sei();
            eeprom_busy_wait();
        }

        IncrementAddress();

        /* Send confirmation byte back to the host */
        CdcSendByte('\r');
    }
    else if (Command == AVR109_COMMAND_ReadEEPROM)
    {
        /* Read the EEPROM byte and write it to the endpoint */
        CdcSendByte( ValidateEepromBlock(1) ? eeprom_read_byte((uint8_t*)((intptr_t)avr910Address)) : 0xffu );
        IncrementAddress();
    }
#endif
    else if (Command != AVR109_COMMAND_Sync)
    {
        /* Unknown (non-sync) command, return fail code */
        CdcSendByte('?');
    }

    /* Select the IN endpoint */
    Endpoint_SelectEndpoint(CDC_TX_EPADDR);

    /* Remember if the endpoint is completely full before clearing it */
    uint8_t wasFull = ! Endpoint_IsReadWriteAllowed();
    // 
    // transfer terminates with a short packet, 
    // or an empty one if the last packet with real data was full.
    //
    if (CdcFlush()) return;
    if (wasFull) 
    {
        //
        // force an empty packet to signal end of transfer
        //
        if (CdcFlush()) return;
    }

    /* Select the OUT endpoint */
    Endpoint_SelectEndpoint(CDC_RX_EPADDR);

    /* Acknowledge the command from the host */
    Endpoint_ClearOUT();
}
