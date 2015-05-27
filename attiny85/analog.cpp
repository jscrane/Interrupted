#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <Arduino.h>

#include "device.h"
#include "analog.h"

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
	cli();
	unsigned v = reading;
	reading = 0xffff;
	enable(false);
	sei();
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
	}
	return 0;
}

void Analog::_mux() {
	byte ref = 0;
	if (_ref == external)
		ref = _BV(REFS0);
	else if (_ref == internal)
		ref = _BV(REFS1);
	ADMUX = ref | pin_to_mux(_pin);
}

void Analog::sleep() {
	ADCSRA &= ~bit(ADEN);
	ACSR |= bit(ACD);
	power_adc_disable();
}

void Analog::wake() {
	power_adc_enable();
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
