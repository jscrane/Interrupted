#include <Arduino.h>

#include "atomic.h"
#include "device.h"
#include "serial.h"
#include "serialin.h"

static SerialIn_ *device;

bool SerialIn_::begin() {
	device = this;

	P1SEL |= BIT1;
	P1SEL2 |= BIT1;
	init();

	return true;
}

void SerialIn_::_enable(bool e) {
	if (e)
		UC0IE |= UCA0RXIE;
	else
		UC0IE &= ~UCA0RXIE;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void serial_rx_isr(void)
{
	if (device && (UC0IFG & UCA0RXIFG))
		device->on_input(UCA0RXBUF);
	__bic_SR_register_on_exit(LPM4_bits | GIE);
}
