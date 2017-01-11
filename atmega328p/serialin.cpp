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
	return true;
}

void SerialIn::_enable(bool e) {
	bitWrite(UCSR0B, RXEN0, e);
	bitWrite(UCSR0B, RXCIE0, e);
}

ISR(USART_RX_vect)
{
	byte b = UDR0;
	if (device)
		device->on_input(b);
}
