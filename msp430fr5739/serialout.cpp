#include <Energia.h>

#include "atomic.h"
#include "device.h"
#include "serial.h"
#include "serialout.h"

static SerialOut_ *device;

bool SerialOut_::begin() {
	device = this;
	init();
	return false;
}

void SerialOut_::_enable(bool enable) {
	Atomic block;
	if (enable)
		UCA0IE |= UCTXIE;
	else
		UCA0IE &= ~UCTXIE;
}

void on_a0_tx(void) {
	if (device) {
		uint8_t b;
		if (device->next(b))
			UCA0TXBUF = b;
	}
}
