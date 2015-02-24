#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "serial.h"
#include "serialin.h"

static SerialIn *device;

void SerialIn::begin() {
	device = this;
	SerialDevice::begin();
	UCSR0B |= bit(RXEN0);
}

void SerialIn::_enable(bool e) {
	if (e)
		UCSR0B |= bit(RXCIE0);
	else
		UCSR0B &= ~bit(RXCIE0);
}

int SerialIn::read() {
	cli();
	if (_n == 0)
		return -1;
	byte b = _rx_buf[_head++];
	_n--;
	if (_head == sizeof(_rx_buf))
		_head = 0;
	sei();
	return b;
}

void SerialIn::on_input(byte b) {
	if (_n < sizeof(_rx_buf)) {
		byte tail = (_head + _n) % sizeof(_rx_buf);
		_rx_buf[tail] = b;
		_n++;
		ready();
	}
}

ISR(USART_RX_vect)
{
	byte b = UDR0;
	if (device)
		device->on_input(b);
}
