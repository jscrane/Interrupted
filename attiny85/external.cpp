#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "external.h"

static External *e0;

ISR(INT0_vect)
{
	if (e0)
		e0->ready();
}

// note: http://forum.arduino.cc/index.php?topic=130606.msg982123#msg982123
// "The only external interrupt that will wake it from sleep mode power 
// down is LOW."
bool External::begin() {
	e0 = this;
	MCUCR &= ~(_BV(ISC01) | _BV(ISC00));
	if (_mode == CHANGE)
		MCUCR |= _BV(ISC00);
	else if (_mode == FALLING)
		MCUCR |= _BV(ISC01);
	else if (_mode == RISING)
		MCUCR |= _BV(ISC01) | _BV(ISC00);

	return true;
}

void External::_enable(bool e) {
	if (e)
		GIMSK |= _BV(INT0);
	else
		GIMSK &= ~_BV(INT0);
}
