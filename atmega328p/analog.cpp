#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <Arduino.h>

#include "device.h"
#include "analog.h"

static Analog *adc;
static volatile unsigned reading = 0xffff;

ISR(ADC_vect) {
	if (adc) {
		uint8_t low = ADCL;
		uint8_t high = ADCH;
		reading = (high << 8) | low;
		if (adc->_next_reading_valid)
			adc->ready();
		else
		{
			// Can't use this conversion so start a new conversion
			ADCSRA |= bit(ADSC) | bit(ADIE);
			adc->_next_reading_valid = true;
		}
	}
}

unsigned Analog::read() {
	unsigned sreg = SREG;
	cli();
	unsigned v = reading;
	reading = 0xffff;
	disable();
	SREG = sreg;
	return v;
}

void Analog::_enable(bool e) {
	if (e)
		ADCSRA |= _BV(ADSC) | _BV(ADIE);
	else
		ADCSRA &= ~(_BV(ADSC) | _BV(ADIE));
}

void Analog::_init() {
	ADMUX = (_ref << 6) | ((_pin - A0) & 0x0f);
}

void Analog::sleep() {
	ADCSRA &= ~_BV(ADEN);
	ACSR |= _BV(ACD);
	power_adc_disable();
}

void Analog::wake() {
	power_adc_enable();
	_next_reading_valid = false;  // Ignore first reading after power on
	ADCSRA |= _BV(ADEN);
	ACSR &= ~_BV(ACD);
}

bool Analog::begin() {
	adc = this;
	unsigned sreg = SREG;
	cli();
	wake();
	_init();
	SREG = sreg;
	return false;
}

unsigned Analog::_sleepmode() {
	return SLEEP_MODE_ADC;
}
