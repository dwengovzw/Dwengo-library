#include "dwengoBluetooth.h"

void initBluetooth() {
	initUsartAdj(BAUD9600);
}

BYTE bluetoothReady() {
	return DataRdyUSART();
}

BYTE readBluetooth() {
	return RCREG;
}

void sendoBluetooth(BYTE data) {
	TXREG = data;
}
