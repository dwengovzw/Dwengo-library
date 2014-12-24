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

#include "dwengoBoard.h"

// Definitions
#define NMBR_ISR_SLOTS 5
typedef FAR_ROM void (*ISRpointer) (void);

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
#define registerISR(x) registerHighISR(x)
void registerHighISR(ISRpointer isr);
void registerLowISR(ISRpointer isr);

/**
 * \brief Deregister an interrupt service routine
 *
 * Deregister an interrupt service routine. Every time an interrupt occures 
 * every registered ISR will be executed. The number of ISRs is limited to 5 by default.
 *
 * @param isr Function pointer to the iterrupt service routine that needs to be deregistered.
 */
#define deregisterISR(x) derigisterHighISR(x)
void deregisterHighISR(ISRpointer isr);
void deregisterLowISR(ISRpointer isr);

HIGHINTDECLARE(DwengoHighPriorityISR)
LOWINTDECLARE(DwengoLowPriorityISR)

#endif // DWENGO_INTERRUPT_H
