#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "serial.h"
#include "serialout.h"

static SerialOut *device;

bool SerialOut::begin() {
	device = this;
	wake();
	UCSR0B |= _BV(TXEN0);
  sleep();
	return false;
}

bool SerialOut::write(char const *ptr) { 
	if (!_tx_ptr) {
		uint8_t sreg = SREG;
		cli();
		_tx_ptr = ptr;
		UCSR0B |= _BV(TXCIE0);
		UDR0 = *_tx_ptr++;
		SREG = sreg;
		enable();
		return true;
	}
	return false;
}

// Output a single character; may be called from interrupt service routine
// If no more characters, then signal that the device is ready
void SerialOut::do_output() {
	byte b = *_tx_ptr;
	if (b) {
		_tx_ptr++;
		bitClear(UCSR0B, TXC0);
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

