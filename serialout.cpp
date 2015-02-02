#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "serial.h"
#include "serialout.h"

static SerialOut *device;

void SerialOut::begin() {
	device = this;
	SerialDevice::begin();
}

void SerialOut::write(char const *ptr) { 
	if (is_enabled() && !_tx_ptr) {
		_tx_ptr = ptr;
		UCSR0B |= bit(UDRIE0);
	}
}

void SerialOut::do_output() {
	byte b = *_tx_ptr;
	if (b) {
		_tx_ptr++;
		UDR0 = b;
	} else {
		_tx_ptr = 0;
		UCSR0B &= ~bit(UDRIE0);
		ready();
	}
}

ISR(USART_UDRE_vect)
{
	if (device)
		device->do_output();
}
