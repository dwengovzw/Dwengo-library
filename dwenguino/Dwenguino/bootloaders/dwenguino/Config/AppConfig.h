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
 *  \brief Application Configuration Header File 
 *
 *  This is a header file which is be used to configure LUFA's
 *  compile time options, as an alternative to the compile time
 *  constants supplied through a makefile.
 *
 *  For information on what each token does, refer to the
 *  \ref Sec_Options section of the application documentation.
 */

/*
* Modified on Dec 23 2017 by Jelle Roets from Dwengo vzw (www.dwengo.org)
*   Bootloader for Dwenguino
*
* Changes made:
*   - use Dwengo USB VID and PID
*   - add Bootloader enter modes
*   - use own internal serial 
*/

#pragma once
//
// This is the only option absolutely required for a working bootloader
//
	#define ENABLE_BLOCK_SUPPORT
// 
// checks if flash / eeprom address + blocksize are valid => not very useful, avrdude doesn't make mistakes + bootloader is protected by lock fuse
//
// #define ENABLE_SECURITY_CHECKS

// Enter bootloader by doubleclick reset
  // #define BOOTENTER_DOUBLERESET
// Enter bootloader mode by holding a button while pressing and releasing reset: available buttons: SWC, SWN, SWE, SWS, SWW
  #define BOOTENTER_SWITCH
  #define BOOT_SWS 

//
// To have a custom USB serial number reported:
//
	// #define CUSTOM_USB_SERIAL
    // #define STRICT_USB_SERIAL

// enable to use own implementation of internal serial number for usb enumeration: see descriptors.c
#define OWN_INTERNAL_SERIAL
#define INTERNAL_SERIAL_BYTES 10
#define INTERNAL_SERIAL_ADDRESS 0x0E

// #define NO_INTERNAL_SERIAL
//
// these options are not required for normal Arduino IDE uploads to work
// there is really no reason to turn them on unless you are using a
// different upload process.
//

    #define ENABLE_FLASH_ERASE_SUPPORT
//	#define ENABLE_EEPROM_BYTE_SUPPORT
//	#define ENABLE_FLASH_BYTE_SUPPORT
//	#define ENABLE_LOCK_BYTE_WRITE_SUPPORT
	#define ENABLE_LOCK_FUSE_READ_SUPPORT

#define DEVICE_VERSION VERSION_BCD(2,0,0)
#define USB_VID 0xd3e0 // Dwengo LLC VID (Non-official) 
#define USB_PID 0x601b // Dwengo bootloader PID

