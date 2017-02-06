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
		uint8_t low = ADCL;
		uint8_t high = ADCH;
		reading = (high << 8) | low;
		adc->ready();
	}
}

unsigned Analog::read() {
	unsigned sreg = SREG;
	cli();
	unsigned v = reading;
	reading = 0xffff;
	enable(false);
	SREG = sreg;
	return v;
}

void Analog::_enable(bool e) {
	if (e)
		ADCSRA |= bit(ADSC) | bit(ADIE);
	else
		ADCSRA &= ~(bit(ADSC) | bit(ADIE));
}

void Analog::_init() {
	ADMUX = (_ref << 6) | ((_pin - A0) & 0x0f);
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
