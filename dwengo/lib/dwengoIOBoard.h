#ifndef DWENGO_IO_BOARD_H
#define DWENGO_IO_BOARD_H

/* I2C addresses */

#define IO_ADDR 0x40

/* IO-BOARD functions */

/**
 * \brief IO-Board initialization function
 *
 * Configure PCA9555 (max 4 IO-boards)
 */
void initIOBoard(void);

/**
 * \brief Read from IO-board
 *
 * @param address the address of used IO-board ( 0 <= address <=3 )
 */
unsigned char readInputs_addr(unsigned char address); 

/**
 * \brief Write to IO-board
 *
 * @param address the address of used IO-board ( 0 <= address <=3 )
 * @param out byte that defines which relais should be set
 */
void setOutputs_addr(unsigned char address, unsigned char out);

/**
 * \brief Read from IO-board
 *
 * Control only one IO-board. Default address = 0.
 */
unsigned char readInputs();

/**
 * \brief Write to IO-board
 *
 * Control only one IO-board. Default address = 0.
 *
 * @param out byte that defines which relais should be set
 */
void setOutputs(unsigned char out);


#endif // DWENGO_IO_BOARD_H