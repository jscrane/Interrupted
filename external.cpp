#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "external.h"

static External *e0, *e1;

ISR(INT0_vect)
{
	if (e0)
		e0->trigger();
}

ISR(INT1_vect)
{
	if (e1)
		e1->trigger();
}

void External::begin() {
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
		EIFR = bit(INTF0);
		EIMSK |= bit(INT0);
	} else {
		e1 = this;
		EICRA &= ~bit(ISC11) & ~bit(ISC10);
		if (_mode == CHANGE)
			EICRA |= bit(ISC10);
		else if (_mode == FALLING)
			EICRA |= bit(ISC11);
		else if (_mode == RISING)
			EICRA |= bit(ISC11) | bit(ISC10);
		EIFR = bit(INTF1);
		EIMSK |= bit(INT1);
	}
	pinMode(_pin, INPUT);
	digitalWrite(_pin, HIGH);	// enable pullup
}

