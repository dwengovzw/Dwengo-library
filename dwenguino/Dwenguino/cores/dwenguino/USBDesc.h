/*
   Copyright (c) 2011, Peter Barrett
   Copyright (c) 2015, Arduino LLC

   Permission to use, copy, modify, and/or distribute this software for
   any purpose with or without fee is hereby granted, provided that the
   above copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
   BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
   OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
   WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
   ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
   SOFTWARE.
 */

/*
  Modified on Dec 20 2014 by Jelle Roets from Dwengo vzw (www.dwengo.org)

  Changes made:
    - add Dwenguino usb descriptors
    - report internal serial number (in signature row of avr) during usb enumeration
*/

#define PLUGGABLE_USB_ENABLED

#if defined(EPRST6)
#define USB_ENDPOINTS 7 // AtMegaxxU4
#else
#define USB_ENDPOINTS 5 // AtMegaxxU2
#endif

#define ISERIAL_MAX_LEN     20

#define CDC_DEVICE_CLASS 0x02
#define CDC_ACM_SUBCLASS 0x02
#define NO_SUBCLASS 0x00
#define NO_PROTOCOL 0x00 

#define CDC_INTERFACE_COUNT	2
#define CDC_ENPOINT_COUNT	3

#define CDC_ACM_INTERFACE	0	// CDC ACM
#define CDC_DATA_INTERFACE	1	// CDC Data
#define CDC_FIRST_ENDPOINT	1
#define CDC_ENDPOINT_ACM	(CDC_FIRST_ENDPOINT)							// CDC First
#define CDC_ENDPOINT_OUT	(CDC_FIRST_ENDPOINT+1)
#define CDC_ENDPOINT_IN		(CDC_FIRST_ENDPOINT+2)

#define INTERFACE_COUNT		(MSC_INTERFACE + MSC_INTERFACE_COUNT)

#define CDC_RX CDC_ENDPOINT_OUT
#define CDC_TX CDC_ENDPOINT_IN

#define IMANUFACTURER   1
#define IPRODUCT        2
#define ISERIAL         3

#define USE_INTERNAL_SERIAL
#define INTERNAL_SERIAL_LENGTH_BYTES 10
#define INTERNAL_SERIAL_START_ADDRESS 0x0E

