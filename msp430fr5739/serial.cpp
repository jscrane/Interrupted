#include <Energia.h>

#include "device.h"
#include "serial.h"

void SerialDevice::init() {
	// the first device to specify a baud-rate gets to initialise
	if (_baud) {
		// see http://longhornengineer.com/code/MSP430/UART/uart.c
		UCA0CTL1 |= UCSSEL_2;
		unsigned long divider = F_CPU / _baud / 16;
		UCA0BR0 = divider;
		UCA0BR1 = divider >> 8;
		UCA0MCTLW = 0x10 | UCOS16;
		UCA0CTL1 &= ~UCSWRST;
	}
}

unsigned SerialDevice::_sleepmode() {
	return LPM4_bits;
}

extern void on_a0_rx(void);
extern void on_a0_tx(void);

#pragma vector=USCI_A0_VECTOR
__interrupt void serial_rx_isr(void)
{
	if (UCA0IFG & UCRXIFG)
		on_a0_rx();
	if (UCA0IFG & UCTXIFG)
		on_a0_tx();
	__bic_SR_register_on_exit(LPM4_bits | GIE);
}
