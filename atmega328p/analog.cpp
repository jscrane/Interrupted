#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <Arduino.h>

#include "atomic.h"
#include "device.h"
#include "analog.h"

static Device *adc;
static volatile unsigned reading = 0xffff;

ISR(ADC_vect) {
	if (adc) {
		uint8_t low = ADCL;
		uint8_t high = ADCH;
		reading = (high << 8) | low;
		adc->ready();
	}
}

unsigned Analog::read() {
	Atomic block;
	unsigned v = reading;
	reading = 0xffff;
	disable();
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

bool Analog::begin() {
	Atomic block;
	adc = this;
	power_adc_enable();
	ADCSRA |= _BV(ADEN);
	ACSR &= ~_BV(ACD);
	_init();
	return false;
}

unsigned Analog::_sleepmode() {
	return SLEEP_MODE_ADC;
}
