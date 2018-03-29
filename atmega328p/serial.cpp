#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "device.h"
#include "serial.h"

void SerialDevice::init() {
	if (_baud) {
		power_usart0_enable();
		unsigned prescale = (F_CPU / 4 / _baud - 1) / 2;
		uint8_t sreg = SREG;
		cli();
		UBRR0H = (prescale >> 8) & 0xff;
		UBRR0L = prescale & 0xff;
		UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);	// 8N1
		SREG = sreg;
	}
}

unsigned SerialDevice::_sleepmode() {
	return SLEEP_MODE_IDLE;
}
