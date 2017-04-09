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
	disable();
	sei();
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
	adc = this;
	unsigned sreg = SREG;
	cli();
	power_adc_enable();
	ADCSRA |= bit(ADEN);
	ACSR &= ~bit(ACD);
	_init();
	SREG = sreg;
	return false;
}

unsigned Analog::_sleepmode() {
	return SLEEP_MODE_ADC;
}
