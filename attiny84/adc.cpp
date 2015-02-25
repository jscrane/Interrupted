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
		ADCSRA |= bit(ADSC) | bit(ADIE);
	else
		ADCSRA &= ~(bit(ADSC) | bit(ADIE));
}

void Analog::_mux() {
	byte ref = bit(REFS0);
	if (_ref == internal)
		ref |= bit(REFS1);
	else if (_ref == external)
		ref = 0;
	byte mux = 0;
	switch (_pin) {
	case A0:
		mux = 0;
		break;
	case A1:
		mux = 1;
		break;
	case A2:
		mux = 2;
		break;
	case A3:
		mux = 3;
		break;
	case A4:
		mux = 4;
		break;
	case A5:
		mux = 5;
		break;
	case A6:
		mux = 6;
		break;
	case A7:
		mux = 7;
		break;
	}
	ADMUX = ref | mux;
	pinMode(_pin, INPUT);
}

bool Analog::begin() {
	adc = this;
	power_adc_enable();
	_mux();
	ADCSRA |= bit(ADEN);
	ACSR &= ~bit(ACD);
	return false;
}

unsigned Analog::sleepmode() {
	return SLEEP_MODE_ADC;
}
