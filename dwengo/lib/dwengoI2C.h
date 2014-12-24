/*
 * I2C
 * part of Dwengo library
 *
 * Routines to communicate over an I2C bus
 *
 * Version: 1.0.$Revision: 1 $
 * Date: $Date: 2010-05-01 23:09:41 +0200 (za, 01 mei 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

/**
 * \file
 * \brief I2C routines.
 *
 * Library to communicate over an I2C bus.
 */

#ifndef I2C_H
#define I2C_H

#include "dwengoBoard.h"

#define I2C_READ 1
#define I2C_WRITE 0

/* I2C low-level functions */

/**
 * \brief Send I2C start condition and address
 *
 * @param addr, rw
 */
void startI2C(BYTE addr, char rw);

/**
 * \brief Write a single byte
 *
 * @param val
 */
void writeI2C(BYTE val);

/**
 * \brief Read a single byte, optionally send ACK
 *
 * @param ack
 */
unsigned char readI2C(char ack);

/**
 * \brief Send I2C stop condition
 *
 * Send I2C stop condition
 */
void stopI2C(void);


/* I2C high-level functions */

/**
 * \brief Initialize I2C hardware
 *
 * Initialize I2C hardware
 */
void initI2C(void);

/**
 * \brief Write single byte to address
 *
 * @param addr, val
 */
void write1I2C(BYTE addr, BYTE val);

/**
 * \brief Write two bytes to address
 *
 * @param addr, val1, val2
 */
void write2I2C(BYTE addr, BYTE val1, BYTE val2);

/**
 * \brief Read and return single byte to address
 *
 * @param addr
 */
int read1I2C(BYTE addr);

/**
 * \brief Read two bytes from address and return as 16-bit value
 *
 * @param addr
 */
int read2I2C(BYTE addr);

#endif // I2C_H
