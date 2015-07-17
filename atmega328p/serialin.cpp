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
	UCSR0B |= bit(RXEN0);
	return true;
}

void SerialIn::_enable(bool e) {
	if (e)
		UCSR0B |= bit(RXCIE0);
	else
		UCSR0B &= ~bit(RXCIE0);
}

ISR(USART_RX_vect)
{
	byte b = UDR0;
	if (device)
		device->on_input(b);
}
