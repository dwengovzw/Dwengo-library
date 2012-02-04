/*
 * USART
 * part of Dwengo library
 *
 * USART routines for the dwengo board.
 *
 * Version: 1.0.$Revision: 3982 $
 * Date: $Date: 2011-06-05 16:23:01 +0200 (Sun, 05 Jun 2011) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

#include "dwengoUsart.h"
#include <stdlib.h>

void initUsart() {
  initUsartAdj(BAUD9600);
}

void initUsartAdj(char bautrate) {

  //UART pins as input
  TRISCbits.TRISC6 = 1;
  TRISCbits.TRISC7 = 1;

  //Baud rate
  switch(bautrate) {
    case BAUD300:
    case BAUD1200:
    case BAUD2400:
    case BAUD9600:
    case BAUD19200:
    case BAUD57600:
    case BAUD115200:
      BAUDCONbits.BRG16=1;
      TXSTAbits.BRGH = 1;
      break;
    default:
      BAUDCONbits.BRG16=1;
      TXSTAbits.BRGH = 1;
  }

  switch(bautrate) {
    case BAUD300:
     SPBRGH = 0x9C;
     SPBRG  = 0x3F;
     break;
    case BAUD1200:
     SPBRGH = 0x27;
     SPBRG  = 0x0F;
     break;
    case BAUD2400:
     SPBRGH = 0x13;
     SPBRG  = 0x87;
     break;
    case BAUD9600:
     SPBRGH = 0x04;
     SPBRG  = 0xE1;
     break;
    case BAUD19200:
     SPBRGH = 0x02;
     SPBRG  = 0x70;
     break;
    case BAUD57600:
     SPBRGH = 0x00;
     SPBRG  = 0xCF;
     break;
    case BAUD115200:
     SPBRGH = 0x00;
     SPBRG  = 0x67;
     break;
    default:
     SPBRGH = 0x04;
     SPBRG  = 0xE1;
  }

  TXSTAbits.SYNC = 0;
  RCSTAbits.SPEN = 1;

  //Transmit
  BAUDCONbits.TXCKP = 0;

  PIE1bits.TXIE = 0;
 
  TXSTAbits.TX9 = 0;

  TXSTAbits.TXEN = 1;

  //Receive
  BAUDCONbits.RXDTP = 0;
  
  PIE1bits.RCIE = 0;
  
  RCSTAbits.RX9 = 0;

  RCSTAbits.CREN = 1;
}


void printStringToUSART(const far rom char* message) {
  putrsUSART(message);
}

void putByteToUSART(BYTE data) {
	TXREG = data;
}