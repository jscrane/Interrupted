#include <Energia.h>

#include "device.h"
#include "analog.h"

static Device *adc;

/*
 * See wiring_analog.c, analogRead()
 */
#pragma vector=ADC10_VECTOR 
__interrupt void adc_isr(void)
{
	if (adc)
		adc->ready();
	__bic_SR_register_on_exit(LPM0_bits | GIE);
}

unsigned Analog::read() {
	noInterrupts();
	unsigned v = ADC10MEM;
	disable();
	interrupts();
	return v;
}

void Analog::_enable(bool e) {
	if (e)
		// enable ADC and start conversion
		ADC10CTL0 |= ENC | ADC10SC;
	else
		// disable ADC
		ADC10CTL0 &= ~ENC;
}

void Analog::_init() {
	// handle temperature sensor on A10
	uint8_t channel = _pin > 127? _pin - 128: digitalPinToADCIn(_pin);

	// ADC10OSC as ADC10CLK (~5MHz) / 5
	ADC10CTL1 = ADC10SSEL_0 | ADC10DIV_4 | (channel << 12);

	// disable input/output buffer on pin
	ADC10AE0 = (1 << channel);

	ADC10CTL0 |= ADC10ON | _ref;
}

bool Analog::begin() {
	adc = this;

	// sample + hold @ 64 × ADC10CLKs, enable interrupts
	ADC10CTL0 = ADC10SHT_3 | ADC10IE;

	_init();
	return false;
}

unsigned Analog::_sleepmode() {
	return LPM0_bits;
}
