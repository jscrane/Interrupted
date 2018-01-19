#include <Energia.h>

#include "atomic.h"
#include "device.h"
#include "serial.h"
#include "serialout.h"

static SerialOut *device;

bool SerialOut::begin() {
	device = this;
	init();
	return false;
}

bool SerialOut::write(char const *ptr) {
	Atomic block;
	if (!_tx_ptr) {
		_tx_ptr = ptr;
		UCA0IE |= UCTXIE;
		enable();
	}
}

void SerialOut::do_output() {
	byte b = *_tx_ptr;
	if (b) {
		_tx_ptr++;
		UCA0TXBUF = b;
	} else {
		_tx_ptr = 0;
		UCA0IE &= ~UCTXIE;
		ready();
	}
}

void on_a0_tx(void) {
	if (device)
		device->do_output();
}
