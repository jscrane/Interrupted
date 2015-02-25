#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "serial.h"

void SerialDevice::init() {
	if (_baud) {
		unsigned prescale = ((F_CPU) / 16 + (_baud / 2)) / _baud - 1;
		UBRR0H = (prescale >> 8) & 0xff;
		UBRR0L = prescale & 0xff;
		UCSR0C = bit(UCSZ00) | bit(UCSZ01);
	}
}

unsigned SerialDevice::sleepmode() {
	return SLEEP_MODE_IDLE;
}
