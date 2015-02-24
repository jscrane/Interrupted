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
	return v;
}

void Analog::enable(bool e) {
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
	ADMUX = ref | ((_pin - A0) & 0x0f);
	pinMode(_pin, INPUT);
}

void Analog::pin(int pin) {
	bool e = is_enabled();
	if (e)
		enable(false);

	_pin = pin;
	_mux();

	if (e)
		enable(true);
}

void Analog::begin() {
	adc = this;
	power_adc_enable();
	_mux();
	ADCSRA |= bit(ADEN);
	ACSR &= ~bit(ACD);
}

unsigned Analog::sleepmode() {
	return SLEEP_MODE_ADC;
}
