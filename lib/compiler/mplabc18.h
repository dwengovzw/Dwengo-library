#ifndef COMP_MPLABC18_H
#define COMP_MPLABC18_H
#include <p18f4550.h>

#define FAR_ROM far rom


//Interrupts
//since mplabc18 does add newlines this should be fine
#define HIGHINTERRUPT(name) \
void name();\
#pragma code high_vector=0x08 \
void high_vector() {\
	_asm\
		goto name\
	_endasm\
}\
#pragma code\
#pragma interrupt name\
void name()

#define HIGHINTDECLARE(name) \
void name();

#define LOWINTERRUPT(name) \
void name();\
#pragma code low_vector=0x18 \
void low_vector() {\
	_asm\
		goto name\
	_endasm\
}\
#pragma code\
#pragma interruptlow name\
void name()

#define LOWINTDECLARE(name) \
void name();

//ADC
#define ADCCHAN(x) ADC_CH##x

#endif
