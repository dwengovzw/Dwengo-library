/*
 * Bluetooth
 * part of Dwengo library
 *
 * Send and receive data using the Dwengo Bluetooth module
 *
 * Version: 1.0.$Revision: 2141 $
 * Date: $Date: 2011-05-22 21:26:28 +0200 (di, 04 mei 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

/**
 * \file
 * \brief Bluetooth routines.
 *
 * Library to communicate using a Bluetooth module.
 */

#ifndef DWENGO_BLUETOOTH_H
#define DWENGO_BLUETOOTH_H

#include "dwengoUsart.h"
#include "dwengoBoard.h"

/**
 * \brief Bluetooth initialization function
 *
 * Configures baudrate (= 9600) and pins RC6 and RC7
 */
void initBluetooth(void);

/**
 * \brief Data ready on bluetooth
 *
 * Checks wether data can be send using the bluetooth module.
 *
 */
BYTE bluetoothReady(void);

/**
 * \brief Read from Bluetooth
 *
 * Returns the next byte received by the bluetooth module, can only be used if bluetoothReady() == TRUE
 *
 */
BYTE readBluetooth(void);

/**
 * \brief Send one byte using Bluetooth
 *
 * Sends one byte using the Bluetooth module.
 *
 * @param data the byte to send
 */
void sendBluetooth(BYTE data);

#endif // end DWENGO_BLUETOOTH_H