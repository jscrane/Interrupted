#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "serial.h"

void SerialDevice::init() {
	if (_baud) {
		unsigned prescale = ((F_CPU) / 16 + (_baud / 2)) / _baud - 1;
		uint8_t sreg = SREG;
		cli();
		UBRR0H = (prescale >> 8) & 0xff;
		UBRR0L = prescale & 0xff;
		UCSR0C = bit(UCSZ00) | bit(UCSZ01);	// 8N1
		SREG = sreg;
	}
}

unsigned SerialDevice::_sleepmode() {
	return SLEEP_MODE_IDLE;
}
