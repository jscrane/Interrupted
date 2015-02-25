#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <Arduino.h>

#include "device.h"
#include "adc.h"

static Device *adc;
static volatile unsigned reading = 0xffff;

ISR(ADC_vect) {
	if (adc) {
		byte low, high;
		low = ADCL;
		high = ADCH;
		reading = (high << 8) | low;
		adc->ready();
	}
}

unsigned Analog::read() {
	unsigned v = reading;
	reading = 0xffff;
	enable(false);
	return v;
}

void Analog::_enable(bool e) {
	if (e)
		ADCSRA |= _BV(ADSC) | _BV(ADIE);
	else
		ADCSRA &= ~(_BV(ADSC) | _BV(ADIE));
}

static byte pin_to_mux(byte pin) {
	switch (pin) {
	case A0:
		return 0;
	case A1:
		return 1;
	case A2:
		return 2;
	case A3:
		return 3;
	case A4:
		return 4;
	case A5:
		return 5;
	case A6:
		return 6;
	case A7:
		return 7;
	}
	return 0;
}

void Analog::_mux() {
	byte ref = _BV(REFS0);
	if (_ref == internal)
		ref |= _BV(REFS1);
	else if (_ref == external)
		ref = 0;
	ADMUX = ref | pin_to_mux(_pin);
}

bool Analog::begin() {
	adc = this;
	power_adc_enable();
	ACSR &= ~_BV(ACD);
	ADCSRA = _BV(ADEN);
	ADCSRB = pin_to_mux(_pin);
	_mux();
	return false;
}

unsigned Analog::sleepmode() {
	return SLEEP_MODE_ADC;
}
