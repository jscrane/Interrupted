#include <Energia.h>

#include "device.h"
#include "serial.h"
#include "serialin.h"

static SerialIn *device;

bool SerialIn::begin() {
	device = this;
	init();
	return true;
}

void SerialIn::_enable(bool e) {
	if (e)
		UCA0IE |= UCRXIE;
	else
		UCA0IE &= ~UCRXIE;
}

void on_a0_rx(void) {
	if (device)
		device->on_input(UCA0RXBUF);
}
