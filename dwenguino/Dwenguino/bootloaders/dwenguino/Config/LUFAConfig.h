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
 *  \brief LUFA Library Configuration Header File
 *
 *  This header file is used to configure LUFA's compile time options,
 *  as an alternative to the compile time constants supplied through
 *  a makefile.
 *
 *  For information on what each token does, refer to the LUFA
 *  manual section "Summary of Compile Tokens".
 */

#ifndef _LUFA_CONFIG_H_
#define _LUFA_CONFIG_H_

	#if (ARCH == ARCH_AVR8)

		/* Non-USB Related Configuration Tokens: */
//		#define DISABLE_TERMINAL_CODES

		/* USB Class Driver Related Tokens: */
//		#define HID_HOST_BOOT_PROTOCOL_ONLY
//		#define HID_STATETABLE_STACK_DEPTH       {Insert Value Here}
//		#define HID_USAGE_STACK_DEPTH            {Insert Value Here}
//		#define HID_MAX_COLLECTIONS              {Insert Value Here}
//		#define HID_MAX_REPORTITEMS              {Insert Value Here}
//		#define HID_MAX_REPORT_IDS               {Insert Value Here}
//		#define NO_CLASS_DRIVER_AUTOFLUSH

		/* General USB Driver Related Tokens: */
		#define ORDERED_EP_CONFIG
		#define USE_STATIC_OPTIONS               (USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)
		#define USB_DEVICE_ONLY
//		#define USB_HOST_ONLY
//		#define USB_STREAM_TIMEOUT_MS            {Insert Value Here}
//		#define NO_LIMITED_CONTROLLER_CONNECT
		#define NO_SOF_EVENTS

		/* USB Device Mode Driver Related Tokens:
		   If none of the next 3 are defined, an extra argument is passed to the get descriptor callback
		   so it can return the memory space to which the address refers.
		   Tried putting descriptors only in PROGMEM and switching to USE_FLASH_DESCRIPTORS, but that adds
		   about 200 bytes to flash usage.
		*/
		#define USE_RAM_DESCRIPTORS
//		#define USE_FLASH_DESCRIPTORS
//		#define USE_EEPROM_DESCRIPTORS
//
// define this to match what is used in the Arduino core
//
		#define FIXED_CONTROL_ENDPOINT_SIZE      64
		#define DEVICE_STATE_AS_GPIOR            0
		#define FIXED_NUM_CONFIGURATIONS         1
//		#define CONTROL_ONLY_DEVICE
//		#define INTERRUPT_CONTROL_ENDPOINT
		#define NO_DEVICE_REMOTE_WAKEUP
		#define NO_DEVICE_SELF_POWER

		/* USB Host Mode Driver Related Tokens: */
//		#define HOST_STATE_AS_GPIOR              {Insert Value Here}
//		#define USB_HOST_TIMEOUT_MS              {Insert Value Here}
//		#define HOST_DEVICE_SETTLE_DELAY_MS	     {Insert Value Here}
//		#define NO_AUTO_VBUS_MANAGEMENT
//		#define INVERTED_VBUS_ENABLE_LINE

	#else

		#error Unsupported architecture for this LUFA configuration file.

	#endif
#endif
