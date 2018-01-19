#include <Energia.h>

#include "atomic.h"
#include "device.h"
#include "analog.h"

static Device *adc;
static unsigned value;

/*
 * See wiring_analog.c, analogRead()
 */
#pragma vector=ADC10_VECTOR 
__interrupt void adc_isr(void)
{
	if (adc && !(ADC10CTL1 & ADC10BUSY)) {
		value = ADC10MEM0;
		adc->ready();
	}
	__bic_SR_register_on_exit(LPM0_bits | GIE);
}

unsigned Analog::read() {
	Atomic block;
	unsigned v = value;
	disable();
	return v;
}

void Analog::_enable(bool e) {
	if (e)
		// enable ADC and start conversion
		ADC10CTL0 |= ADC10ENC | ADC10SC;
	else
		// disable ADC
		ADC10CTL0 &= ~ADC10ENC;
}

// from wiring_analog.c
#define REFV_MASK 0x70
#define REF_MASK 0x31

void Analog::_init() {
	// handle temperature sensor on A10
	uint8_t channel = _pin > 127? _pin - 128: digitalPinToADCIn(_pin);

	// ADC10OSC as ADC10CLK (~5MHz) / 5, ADCCLK = MODOSC; sampling timer
	ADC10CTL1 = ADC10SSEL_0 | ADC10DIV_4 | ADC10SHP;

	// 10-bit resolution
	ADC10CTL2 |= ADC10RES;

	// set channel and reference
	ADC10MCTL0 = channel | (_ref & REFV_MASK);

	while (REFCTL0 & REFGENBUSY);
	REFCTL0 |= _ref & REF_MASK;
	ADC10CTL0 |= ADC10ON | ADC10SHT_4;
}

bool Analog::begin() {
	adc = this;

	// sample + hold @ 64 × ADC10CLKs, enable interrupts
	ADC10CTL0 = ADC10SHT_4;

	// enable interrupts
    	ADC10IE |= ADC10IE0;

	_init();
	return false;
}

unsigned Analog::_sleepmode() {
	return LPM0_bits;
}
