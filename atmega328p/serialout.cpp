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
	return false;
}

void SerialOut::_enable(bool e) {
	bitClear(UCSR0B, TXCIE0);
	bitWrite(UCSR0B, TXEN0, e);
}

// Prepare for transmission of characters from the given buffer
void SerialOut::write(char const *ptr) {
	if (is_enabled() && !_tx_ptr) {
		uint8_t saved_status = SREG;
		cli();
		_tx_ptr = ptr;
		bitSet(UCSR0B, UDRIE0);
		bitSet(UCSR0B, TXCIE0);
		SREG = saved_status;
	}
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
		UCSR0B &= ~bit(UDRIE0);
		if (bitRead(UCSR0B, TXC0))
		{
			bitClear(UCSR0B, TXCIE0);  // Don't need this interrupt now - the transmission finished while in this routine
			ready();
		}
	}
}

// When the transmitter data register is empty, output the next byte
ISR(USART_UDRE_vect)
{
	if (device)
		device->do_output();
}

// When the transmission is complete, the device is ready
ISR(USART_TXC_vect)
{
	if (device && !device->transmitting())
	{
		device->ready();
	}
}
