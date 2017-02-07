#include <Energia.h>

#include "device.h"
#include "serial.h"
#include "serialout.h"

static SerialOut *device;

bool SerialOut::begin() {
	device = this;

	P2SEL1 |= BIT0;
	P2SEL0 &= ~BIT0;
	init();

	return false;
}

bool SerialOut::write(char const *ptr) {
	if (!_tx_ptr) {
		noInterrupts();
		_tx_ptr = ptr;
		UCA0IE |= UCTXIE;
		enable();
		interrupts();
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
