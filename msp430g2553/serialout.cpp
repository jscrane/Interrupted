#include <Energia.h>

#include "device.h"
#include "serial.h"
#include "serialout.h"

static SerialOut *device;

bool SerialOut::begin() {
	device = this;

	P1SEL |= BIT2;
	P1SEL2 |= BIT2;
	init();

	return false;
}

bool SerialOut::write(char const *ptr) {
	Atomic block;
	if (!_tx_ptr) {
		_tx_ptr = ptr;
		UC0IE |= UCA0TXIE;
		enable();
		return true;
	}
	return false;
}

void SerialOut::do_output() {
	byte b = *_tx_ptr;
	if (b) {
		_tx_ptr++;
		UCA0TXBUF = b;
	} else {
		_tx_ptr = 0;
		UC0IE &= ~UCA0TXIE;
		ready();
	}
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void serial_tx_isr(void)
{
	if (device && (UC0IFG & UCA0TXIFG))
		device->do_output();
	__bic_SR_register_on_exit(LPM4_bits | GIE);
}
