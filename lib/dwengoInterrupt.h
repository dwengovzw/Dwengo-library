/*
 * Interrupt
 * part of Dwengo library
 * 
 * Managing interupt service routines
 *
 * Version: 1.0.$Revision: 2121 $
 * Date: $Date: 2010-05-01 23:09:41 +0200 (Sat, 01 May 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

 /**
  * \file
  * \brief Interrupt management routines.
  *
  * Librairy to dynamically manage interupt service routines.
  */


#ifndef DWENGO_INTERRUPT_H
#define DWENGO_INTERRUPT_H

#include <p18f4550.h> 
#include "dwengoBoard.h"

// Definitions
#define NMBR_ISR_SLOTS 5
typedef far rom void (*ISRpointer) (void);

// Functions
/**
 * \brief Initialize interrupt functionality
 *
 * This routine initializes interrupt functionality for Dwengo board. Sets some global variables.
 */
void initInterrupt(void);

/**
 * \brief Register an interrupt service routine
 *
 * Register an interrupt service routine. Every time an interrupt occures 
 * every registered ISR will be executed. The number of ISRs is limited to 5 by default.
 *
 * @param isr Function pointer to the iterrupt service routine that needs to be registered.
 */
void registerISR(ISRpointer isr);

/**
 * \brief Deregister an interrupt service routine
 *
 * Deregister an interrupt service routine. Every time an interrupt occures 
 * every registered ISR will be executed. The number of ISRs is limited to 5 by default.
 *
 * @param isr Function pointer to the iterrupt service routine that needs to be deregistered.
 */
void deregisterISR(ISRpointer isr);


#endif // DWENGO_INTERRUPT_H