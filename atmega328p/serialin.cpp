#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "serial.h"
#include "serialin.h"

static SerialIn *device;

bool SerialIn::begin() {
	device = this;
	init();
	return true;
}

void SerialIn::_enable(bool e) {
	uint8_t m = _BV(RXCIE0) | _BV(RXEN0);
	if (e)
		UCSR0B |= m;
	else
		UCSR0B &= ~m;
}

ISR(USART_RX_vect)
{
	uint8_t b = UDR0;
	if (device)
		device->on_input(b);
}
