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
		EICRA &= ~bit(ISC01) & ~bit(ISC00);
		if (_mode == CHANGE)
			EICRA |= bit(ISC00);
		else if (_mode == FALLING)
			EICRA |= bit(ISC01);
		else if (_mode == RISING)
			EICRA |= bit(ISC01) | bit(ISC00);
	} else if (_pin == 3) {
		e1 = this;
		EICRA &= ~bit(ISC11) & ~bit(ISC10);
		if (_mode == CHANGE)
			EICRA |= bit(ISC10);
		else if (_mode == FALLING)
			EICRA |= bit(ISC11);
		else if (_mode == RISING)
			EICRA |= bit(ISC11) | bit(ISC10);
	}
	digitalWrite(_pin, HIGH);	// enable pullup
	return true;
}

void External::_enable(bool e) {
	if (_pin == 2) {
		EIFR = bit(INTF0);
		if (e)
			EIMSK |= bit(INT0);
		else
			EIMSK &= ~bit(INT0);
	} else if (_pin == 3) {
		EIFR = bit(INTF1);
		if (e)
			EIMSK |= bit(INT1);
		else
			EIMSK &= ~bit(INT1);
	}
}
