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
		byte low, high;
		low = ADCL;
		high = ADCH;
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
	cli();
	unsigned v = reading;
	reading = 0xffff;
	enable(false);
	sei();
	return v;
}

void Analog::_enable(bool e) {
	if (e)
		ADCSRA |= bit(ADSC) | bit(ADIE);
	else
		ADCSRA &= ~(bit(ADSC) | bit(ADIE));
}

void Analog::_mux() {
	ADMUX = (_ref << 6) | ((_pin - A0) & 0x0f);
}

void Analog::sleep() {
	ADCSRA &= ~bit(ADEN);
	ACSR |= bit(ACD);
	power_adc_disable();
}

void Analog::wake() {
	power_adc_enable();
	_next_reading_valid = false;  // Ignore first reading after power on
	ADCSRA |= bit(ADEN);
	ACSR &= ~bit(ACD);
}

bool Analog::begin() {
	adc = this;
	wake();
	_mux();
	return false;
}

unsigned Analog::_sleepmode() {
	return SLEEP_MODE_ADC;
}
