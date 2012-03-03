#ifndef COMP_SDCC_PIC16_H
#define COMP_SDCC_PIC16_H
#include <pic18fregs.h>

// Keyword to reserve memorry in the far rom section
#define FAR_ROM __far

// C99 compatible assembler keywords
#define _asm __asm
#define _endasm __endasm

// Slight difference in naming
#define NOT_RBPU RBPU

//ugly workaround since sdcc sees "data" as a keyword
#define data Data


//Interrupts
#define HIGHINTERRUPT(name) \
void name() __interrupt 1

#define HIGHINTDECLARE(name) \
void name() __interrupt 1;

#define LOWINTERRUPT(name) \
void name() __interrupt 2

#define LOWINTDECLARE(name) \
void name() __interrupt 2;

//ADC
#define ConvertADC() adc_conv()
#define BusyADC() adc_busy()
#define ReadADC() adc_read()
#define SetChanADC(chan) adc_setchannel(chan)
#define ADCCHAN(x) ADC_CHN_##x

//USART
#define putrsUSART(message) usart_puts(message)
#define DataRdyUSART() usart_drdy()

#endif
