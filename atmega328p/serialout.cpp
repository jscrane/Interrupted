#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "serial.h"
#include "serialout.h"

static SerialOut *device;

bool SerialOut::begin() {
	device = this;
	init();
	return false;
}

void SerialOut::write(char const *ptr) { 
	if (is_enabled() && !_tx_ptr) {
		uint8_t sreg = SREG;
		cli();
		_tx_ptr = ptr;
		UCSR0B |= _BV(UDRIE0) | _BV(TXCIE0);
		SREG = sreg;
	}
}

void SerialOut::do_output() {
	byte b = *_tx_ptr;
	if (b) {
		_tx_ptr++;
		UCSR0B &= ~_BV(TXC0);
		UDR0 = b;
	} else {
		_tx_ptr = 0;
		UCSR0B &= ~_BV(UDRIE0);
		if (UCSR0B & TXC0) {
			UCSR0B &= ~_BV(TXCIE0);
			ready();
		}
	}
}

// when transmitter data register empty, output the next byte
ISR(USART_UDRE_vect)
{
	if (device)
		device->do_output();
}

// when transmission complete, device is ready
ISR(USART_TCX_vect)
{
	if (device && !device->is_transmitting())
		device->ready();
}
