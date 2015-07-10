#include <Energia.h>

#include "device.h"
#include "analog.h"

static Device *adc;
static volatile uint16_t reading = 0xffff;

/*
 * See wiring_analog.c, analogRead()
 */
#pragma vector=ADC10_VECTOR 
__interrupt void adc_isr(void)
{
	if (adc) {
		reading = ADC10MEM;
		adc->ready();
	}
	__bic_SR_register_on_exit(LPM0_bits | GIE);
}

unsigned Analog::read() {
	noInterrupts();
	unsigned v = reading;
	reading = 0xffff;
	disable();
	interrupts();
	return v;
}

void Analog::_enable(bool e) {
	if (e)
		// enable ADC, start conversion, enable interrupts
		ADC10CTL0 |= ENC | ADC10SC | ADC10IE;
	else
		// disable ADC and interrupts
		ADC10CTL0 &= ~(ENC | ADC10IE);
}

// FIXME: reading from temperature sensor
void Analog::_mux() {
	uint8_t channel = digitalPinToADCIn(_pin);

	// ADC10OSC as ADC10CLK (~5MHz) / 5
	ADC10CTL1 = ADC10SSEL_0 | ADC10DIV_4 | (channel << 12);

	// set analog reference, sample + hold @ 64 × ADC10CLKs
	ADC10CTL0 = _ref | ADC10SHT_3;

	// disable input/output buffer on pin
	ADC10AE0 = (1 << channel);
}

void Analog::sleep() {
	// turn off ADC and reference voltage
	ADC10CTL0 &= ~(ADC10ON | REFON);
}

void Analog::wake() {
	ADC10CTL0 |= ADC10ON | REFON;
}

bool Analog::begin() {
	adc = this;
	wake();
	_mux();
	return false;
}

unsigned Analog::_sleepmode() {
	return LPM0_bits;
}
