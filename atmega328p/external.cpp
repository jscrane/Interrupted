#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "external.h"

static External *e0, *e1;

ISR(INT0_vect)
{
	if (e0)
		e0->ready();
}

ISR(INT1_vect)
{
	if (e1)
		e1->ready();
}

bool External::begin() {
	// enable external interrupt and trigger on specified mode
	if (_pin == 2) {
		e0 = this;
		EICRA &= ~_BV(ISC01) & ~_BV(ISC00);
		if (_mode == CHANGE)
			EICRA |= _BV(ISC00);
		else if (_mode == FALLING)
			EICRA |= _BV(ISC01);
		else if (_mode == RISING)
			EICRA |= _BV(ISC01) | _BV(ISC00);
	} else if (_pin == 3) {
		e1 = this;
		EICRA &= ~_BV(ISC11) & ~_BV(ISC10);
		if (_mode == CHANGE)
			EICRA |= _BV(ISC10);
		else if (_mode == FALLING)
			EICRA |= _BV(ISC11);
		else if (_mode == RISING)
			EICRA |= _BV(ISC11) | _BV(ISC10);
	}
	return true;
}

void External::_enable(bool e) {
	if (_pin == 2) {
		EIFR = _BV(INTF0);
		if (e)
			EIMSK |= _BV(INT0);
		else
			EIMSK &= ~_BV(INT0);
	} else if (_pin == 3) {
		EIFR = _BV(INTF1);
		if (e)
			EIMSK |= _BV(INT1);
		else
			EIMSK &= ~_BV(INT1);
	}
}
