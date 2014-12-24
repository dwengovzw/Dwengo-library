#include <dwengoConfig.h>
#include <dwengoBoard.h>
#include <dwengoUsart.h>

void main(void) {

  initBoard();
  initUsartAdj(BAUD115200);

  while(TRUE){
    while(PIR1bits.RCIF==0) {}
    while(PIR1bits.TXIF==0) {}
    TXREG = RCREG;
  }
}