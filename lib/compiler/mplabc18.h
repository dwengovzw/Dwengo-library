#ifndef COMP_MPLABC18_H
#define COMP_MPLABC18_H
#include <p18f4550.h>

#define FAR_ROM far rom


//Interrupts

#define HIGHINTERRUPT(name) \
void name()

#define HIGHINTDECLARE(name) \
void name();

#define LOWINTERRUPT(name) \
void name()

#define LOWINTDECLARE(name) \
void name();

//ADC
#define ADCCHAN(x) ADC_CH##x

#endif
