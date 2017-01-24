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
		uint8_t low, high;
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
