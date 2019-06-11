#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "device.h"
#include "serial.h"

void SerialDevice::init() {
	if (_baud) {
		power_usart0_enable();
		uint16_t baud_setting = (F_CPU / 4 / _baud - 1) / 2;
		uint8_t sreg = SREG;
		cli();
		UCSR0A = _BV(U2X0);
		UBRR0H = baud_setting >> 8;
		UBRR0L = baud_setting;
		UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);	// 8N1
		SREG = sreg;
	}
}

unsigned SerialDevice::_sleepmode() {
	return SLEEP_MODE_IDLE;
}
