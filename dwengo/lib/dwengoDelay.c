/*
 * Delay
 * part of Dwengo library
 *
 * Delay routines for the dwengo board.
 *
 * Version: 1.0.$Revision: 1933 $
 * Date: $Date: 2010-03-14 21:43:25 +0100 (Sun, 14 Mar 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

#include "dwengoDelay.h"
#ifdef SDCC_pic16
//fix problem with sdcc-sdcpp not inserting newlines in macros
#define Nop()           do { __asm nop __endasm; } while(0)
void delay_us(char a)
{
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	while(--a) {
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
	}
}
#endif
void delay_100us(int n) {
	int i;
	if (n != 0) {
		delay_us(95);
		for (i = 1; i<n; i++) {
			delay_us(98);
		}
	}
}

void delay_ms(int n){
	int i;
	if (n != 0) {
		delay_us(94);
		delay_100us(9);
		for (i = 1; i<n; i++) {
			delay_us(96);
			delay_100us(9);
		}
	}
}

void delay_s(int n){
    int i;
	for (i=0; i<n; i++) {
        delay_ms(1000);
    }
}
