#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <Arduino.h>

#include "atomic.h"
#include "device.h"
#include "analog.h"

static Device *adc;
static volatile uint16_t reading = 0xffff;

ISR(ADC_vect) {
	if (adc) {
		uint8_t low, high;
		low = ADCL;
		high = ADCH;
		reading = (high << 8) | low;
		adc->ready();
	}
}

unsigned Analog::read() {
	Atomic block;
	uint16_t v = reading;
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
	ADMUX = (_ref << 6) | _pin;
}

bool Analog::begin() {
	Atomic block;
	adc = this;
	power_adc_enable();
	ADCSRA |= bit(ADEN);
	ACSR &= ~bit(ACD);
	_init();
	return false;
}

unsigned Analog::_sleepmode() {
	return SLEEP_MODE_ADC;
}
