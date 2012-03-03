/**
 * Interrupt
 * part of Dwengo library
 * 
 * Managing interupt service routines
 *
 * Version: 1.0.$Revision: 2121 $
 * Date: $Date: 2010-05-01 23:09:41 +0200 (Sat, 01 May 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

#include "dwengoInterrupt.h"

ISRpointer highInterruptSlots[NMBR_ISR_SLOTS];
ISRpointer lowInterruptSlots[NMBR_ISR_SLOTS];

HIGHINTERRUPT(DwengoHighPriorityISR) {
	unsigned char i;

	for (i=0; i<NMBR_ISR_SLOTS; i++) {
		if (highInterruptSlots[i]!=0)
			highInterruptSlots[i]();
	}
}

LOWINTERRUPT(DwengoLowPriorityISR) {
	unsigned char i;

	for (i=0; i<NMBR_ISR_SLOTS; i++) {
		if (lowInterruptSlots[i]!=0)
			lowInterruptSlots[i]();
	}
}

void initInterrupt() {
	unsigned char i;
	for (i=0; i < NMBR_ISR_SLOTS; i++)
		highInterruptSlots[i] = 0;
	for (i=0; i < NMBR_ISR_SLOTS; i++)
		lowInterruptSlots[i] = 0;
}

void registerHighISR(ISRpointer a) {
	unsigned char i = 0;
	while(highInterruptSlots[i] != 0)
		i++;
	highInterruptSlots[i] = a;
}

void deregisterHighISR(ISRpointer a) {
	unsigned char i = 0;
	while(highInterruptSlots[i] != a)
		i++;
	highInterruptSlots[i] = 0;
}

void registerLowISR(ISRpointer a) {
	unsigned char i = 0;
	while(lowInterruptSlots[i] != 0)
		i++;
	lowInterruptSlots[i] = a;
}

void deregisterLowISR(ISRpointer a) {
	unsigned char i = 0;
	while(lowInterruptSlots[i] != a)
		i++;
	lowInterruptSlots[i] = 0;
}
