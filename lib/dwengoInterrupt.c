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

void DwengoHighPriorityISR();
void DwengoLowPriorityISR();

ISRpointer interruptSlots[NMBR_ISR_SLOTS];

#pragma code high_vector=0x08
void high_vector() {
	_asm
		goto DwengoHighPriorityISR
	_endasm
}
#pragma code

#pragma code low_vector=0x18
void low_vector() {
	_asm
		goto DwengoLowPriorityISR
	_endasm
}
#pragma code

#pragma interrupt DwengoHighPriorityISR
void DwengoHighPriorityISR() {
  unsigned char i;

  for (i=0; i<NMBR_ISR_SLOTS; i++) {
    if (interruptSlots[i]!=0)
      interruptSlots[i]();
  }
	
}

#pragma interruptlow DwengoLowPriorityISR
void DwengoLowPriorityISR() {

}

void initInterrupt() {
  unsigned char i;
  for (i=0; i < NMBR_ISR_SLOTS; i++) {
    interruptSlots[i] = 0;
  }
}

void registerISR(ISRpointer a) {
  unsigned char i;
  i=0;
  while(interruptSlots[i] != 0) {
  	i++;
  }
  interruptSlots[i] = a;
}

void deregisterISR(ISRpointer a) {
  unsigned char i;
  i=0;
  while(interruptSlots[i] != a) {
  	i++;
  }
  interruptSlots[i] = 0;
}
