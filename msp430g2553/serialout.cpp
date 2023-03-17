#include <Arduino.h>

#include "atomic.h"
#include "device.h"
#include "serial.h"
#include "serialout.h"

static SerialOut_ *device;

bool SerialOut_::begin() {
	device = this;

	P1SEL |= BIT2;
	P1SEL2 |= BIT2;
	init();

	return false;
}

void SerialOut_::_enable(bool enable) {
	if (enable)
		UC0IE |= UCA0TXIE;
	else
		UC0IE &= ~UCA0TXIE;
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void serial_tx_isr(void)
{
	if (device && (UC0IFG & UCA0TXIFG)) {
		uint8_t b;
		if (device->next(b))
			UCA0TXBUF = b;
	}
	__bic_SR_register_on_exit(LPM4_bits | GIE);
}
