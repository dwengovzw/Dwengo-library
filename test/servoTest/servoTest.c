#include <dwengoConfig.h>
#include <dwengoBoard.h>
#include <dwengoServo.h>
#include <dwengoADC.h>


void main(void) {
  unsigned char position;
  initBoard();
  initServo();

  position = 0;
  while (TRUE) {	// Repeat the following forever
    position++;
    setPosition(2,position);
    delay_ms(10);
  }
}
