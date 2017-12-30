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
 
   USB Device Descriptors, for library use when in USB device mode. Descriptors are special
   computer-readable structures which the host requests upon device enumeration, to determine
   the device's capabilities and functions.

   One might think it would be more efficient to just keep descriptors only in flash,
   then copy them out when requested. The LUFA library even has a configuration for that
   and assumes addresses returned point to flash. Problem is, doing that adds about 200
   bytes to the overall flash image. Oh well, best to let sleepin' dogs lie.
 */

/*
* Modified on Dec 23 2017 by Jelle Roets from Dwengo vzw (www.dwengo.org)
*   Bootloader for Dwenguino
*
* Changes made:
*   - specify Dwengo manufacturer and product descriptors
*   
*/

#include "Descriptors.h"

/** Device descriptor structure. This descriptor, located in SRAM memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 */

/**
0x9207 is the PID for LilyPad bootloader, 0x9208 is PID for LilyPad sketches
*/
const USB_Descriptor_Device_t PROGMEM DeviceDescriptor =	// TODO: why are these in PROGMEM?? , also remove Sram versions caches and cache function at the end of this file
{
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(1,1,0),
	.Class                  = CDC_CSCP_CDCClass,
	.SubClass               = CDC_CSCP_NoSpecificSubclass,
	.Protocol               = CDC_CSCP_NoSpecificProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

	.VendorID               = USB_VID,
	.ProductID              = USB_PID,
	.ReleaseNumber          = VERSION_BCD(1,0,0),

	.ManufacturerStrIndex   = STRING_ID_Manufacturer,
	.ProductStrIndex        = STRING_ID_Product,
#if defined(CUSTOM_USB_SERIAL)
	.SerialNumStrIndex      = STRING_ID_Serial, 
#else
	.SerialNumStrIndex      = NO_DESCRIPTOR, 
#endif

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

static uint8_t SramDeviceDescriptor[ sizeof(USB_Descriptor_Device_t) ];

/**
Configuration descriptor structure. This descriptor, located in SRAM memory, describes the usage
of the device in one of its supported configurations, including information about any device interfaces
and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
a configuration so that the host may correctly communicate with the USB device.
 */
const USB_Descriptor_Configuration_t ConfigurationDescriptor =
{
	.Config =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
			.TotalInterfaces        = 2,

			.ConfigurationNumber    = 1,
			.ConfigurationStrIndex  = NO_DESCRIPTOR,

			.ConfigAttributes       = USB_CONFIG_ATTR_RESERVED,

			.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
		},

	.CDC_CCI_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = INTERFACE_ID_CDC_CCI,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 1,

			.Class                  = CDC_CSCP_CDCClass,
			.SubClass               = CDC_CSCP_ACMSubclass,
			.Protocol               = CDC_CSCP_ATCommandProtocol,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},

	.CDC_Functional_Header =
		{
			.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalHeader_t), .Type = DTYPE_CSInterface},
			.Subtype                = 0x00,

			.CDCSpecification       = VERSION_BCD(1,1,0),
		},

	.CDC_Functional_ACM =
		{
			.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalACM_t), .Type = DTYPE_CSInterface},
			.Subtype                = 0x02,

			.Capabilities           = 0x02,
		},

	.CDC_Functional_Union =
		{
			.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalUnion_t), .Type = DTYPE_CSInterface},
			.Subtype                = 0x06,

			.MasterInterfaceNumber  = INTERFACE_ID_CDC_CCI,
			.SlaveInterfaceNumber   = INTERFACE_ID_CDC_DCI,
		},

	.CDC_NotificationEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = CDC_NOTIFICATION_EPADDR,
			.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = CDC_NOTIFICATION_EPSIZE,
			.PollingIntervalMS      = 0xFF
		},

	.CDC_DCI_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = INTERFACE_ID_CDC_DCI,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 2,

			.Class                  = CDC_CSCP_CDCDataClass,
			.SubClass               = CDC_CSCP_NoDataSubclass,
			.Protocol               = CDC_CSCP_NoDataProtocol,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},

	.CDC_DataOutEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = CDC_RX_EPADDR,
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = CDC_TXRX_EPSIZE,
			.PollingIntervalMS      = 0x05
		},

	.CDC_DataInEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = CDC_TX_EPADDR,
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = CDC_TXRX_EPSIZE,
			.PollingIntervalMS      = 0x05
		}
};

/** Language descriptor structure. This descriptor, located in SRAM memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
const USB_Descriptor_String_t LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t ManufacturerString = USB_STRING_DESCRIPTOR(L"Dwengo LLC");

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t ProductString = USB_STRING_DESCRIPTOR(L"Dwenguino Bootloader");

#if defined(CUSTOM_USB_SERIAL) || defined(__DOXYGEN__)

#include "UsbHdwrSerial.h"  // will define the USB_HDWR_SERIAL macro.
/**
 This one has to be in flash so that the sketch can find it and report the proper USB serial number.
 We'll make a copy in SRAM to return to the callback client.
 Rules for valid serial numbers (from Microsoft):
 Length: between 1 and 255 inclusive.
 Valid characters: between 0x20 (ascii space) and 0x7F (ascii), excluding 0x2C (comma).
 Sorry about the ridiculous macros...but that's what's required to paste an "L" infront of the
 quoted string defined by USB_HDWR_SERIAL.
*/

#define _WIDEN(x) L ## x
#define WIDEN(x) _WIDEN(x)
#define WIDE_SN WIDEN( USB_HDWR_SERIAL )

const USB_Descriptor_String_t PROGMEM SerialString = USB_STRING_DESCRIPTOR( WIDE_SN );

/**
A buffer to contain the a usb string descriptor for hdwr serial number, built from the
ASCII string in EEPROM. It's length includes a 2-byte header plus 2 bytes per unicode character.
Not sure if we need to include a terminating null 16-bits but we do anyway just to be safe.

Let the init code zero this array so we can tell from the length field if it's already been setup.
*/
static uint8_t SramSerialString[ sizeof( USB_StdDescriptor_Header_t ) + (sizeof(USB_HDWR_SERIAL) * 2) + 2 ];

#endif

void __attribute__((noinline)) CacheDescriptor( uint8_t *sram, uint8_t *prog )
{
    if (*sram) return;
    uint8_t cnt = pgm_read_byte( prog );
    while (cnt--) *sram++ = pgm_read_byte( prog++ );
}

/** This function is called by the library when in device mode, and must be overridden (see LUFA library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t	wValue,
                                    const uint16_t	wIndex,
                                    const void** const	DescriptorAddress )
{
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

	const void* Address = NULL;
    //
    // save 18 bytes by using uint8_t for Size instead of uint16_t.
    // every size value we return comes from a single byte length field in a descriptor,
    // so it's impossible that any length we return is more than 255.
    //
	uint8_t    Size    = NO_DESCRIPTOR;
    //
    // we save two bytes making this an if..else if...else if... structure compared to switch()
    //
    if (DescriptorType == DTYPE_Device)
    {
        CacheDescriptor( SramDeviceDescriptor, (uint8_t *)&DeviceDescriptor );
        Address = SramDeviceDescriptor;
        Size    = SramDeviceDescriptor[0];
    }
    else if (DescriptorType == DTYPE_Configuration)
    {
        Address = &ConfigurationDescriptor;
        Size    = sizeof(USB_Descriptor_Configuration_t);
    }
    else if (DescriptorType == DTYPE_String)
    {
        if (DescriptorNumber == STRING_ID_Language)
        {
            Address = &LanguageString;
            Size    = LanguageString.Header.Size;
        }
        else if (DescriptorNumber == STRING_ID_Manufacturer)
        {
            Address = &ManufacturerString;
            Size    = ManufacturerString.Header.Size;
        }
        else if (DescriptorNumber == STRING_ID_Product)
        {
            Address = &ProductString;
            Size    = ProductString.Header.Size;
        }
#if defined(CUSTOM_USB_SERIAL)
        else if (DescriptorNumber == STRING_ID_Serial)
        {
            CacheDescriptor( SramSerialString, (uint8_t *)&SerialString );
            Address = SramSerialString;
            Size    = SramSerialString[0];
        }
#endif
    }

	*DescriptorAddress = Address;
	return Size;
}

