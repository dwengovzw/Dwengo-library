/*
 * USART
 * part of Dwengo library
 *
 * Delay routines for the dwengo board.
 *
 * Version: 1.0.$Revision: 3982 $
 * Date: $Date: 2011-06-05 16:23:01 +0200 (Sun, 05 Jun 2011) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */
 /**
  * \file
  * \brief USART routines
  *
  * Abstraction of USART initialisation function
  */

#ifndef DWENGO_USART_H
#define DWENGO_USART_H

#include <usart.h>
#include "dwengoBoard.h"


#define BAUD300    0
#define BAUD1200   1
#define BAUD2400   2
#define BAUD9600   3
#define BAUD19200  4
#define BAUD57600  5
#define BAUD115200 6

/**
 * \brief Usart initialisation function, baudrate = 9600
 *
 */
void initUsart();

/**
 * \brief Usart initialisation function
 *
 * Initialisation of the UART for an asynchronous serial 
 * connection with adjustable baud rate.
 *
 * @param baudrate The requated baud rate. May be ony of: 
 * BAUD300, BAUD1200, BAUD2400, BAUD9600, BAUD19200, 
 * BAUD57600, BAUD115200.
 */
void initUsartAdj(char baudrate);

/**
 * \brief Write a ROM string to a the USART
 *
 * The string is written to the USART.
 *
 * @param message the NULL-terminated ASCII string to write
 */
void printStringToUSART(const far rom char* message);

/**
 * \brief Write one byte to the USART
 *
 * The data was written to the USART.
 *
 * @param data the byte to send
 */
void putByteToUSART(BYTE data);


#endif // DWENGO_USART_H