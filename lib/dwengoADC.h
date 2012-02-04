/*
 * ADC
 * part of Dwengo library
 *
 * Analog/Digital conversion, AN0-4
 *
 * Version: 1.0.$Revision: 2484 $
 * Date: $Date: 2010-07-26 20:54:16 +0200 (Mon, 26 Jul 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */
/**
 * \file
 * \brief ADC
 *
 * Analog/Digital conversion on the Dwengo board for channel AN0 to channel AN4
 */

#ifndef DWENGO_ADC_H
#define DWENGO_ADC_H

#include "dwengoBoard.h"

// Functions
/**
 * \brief Initialize ADC
 *
 * Configures ADC module so that AN0-AN4 can be used as analog inputs
 */
void initADC(void);   // Call this function to initialise the analog inputs of the board

/**
 * \brief Read analog input
 *
 * Converts an analog input from channel \a address to a 10-bit number.
 *
 * @param address channel to read (has to be in range [0,4] where x corresponds with pin ANx)
 */

int readADC(BYTE address);   // Returns the analog value on channel <address>

#endif // DWENGO_ADC_H
