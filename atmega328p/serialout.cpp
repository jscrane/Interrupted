#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "atomic.h"
#include "device.h"
#include "serial.h"
#include "serialout.h"

static SerialOut *device;

bool SerialOut::begin() {
	device = this;
	init();
	UCSR0B |= _BV(TXEN0);
	return false;
}

bool SerialOut::write(char const *ptr) { 
	Atomic block;
	if (!_tx_ptr) {
		_tx_ptr = ptr;
		UCSR0B |= _BV(TXCIE0);
		UDR0 = *_tx_ptr++;
		enable();
		return true;
	}
	return false;
}

void SerialOut::do_output() {
	byte b = *_tx_ptr;
	if (b) {
		_tx_ptr++;
		UDR0 = b;
	} else {
		_tx_ptr = 0;
		UCSR0B &= ~_BV(TXCIE0);
		ready();
	}
}

ISR(USART_TX_vect)
{
	if (device)
		device->do_output();
}
