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
	// enable external interrupt and trigger on falling edge
	if (_pin == 2) {
		e0 = this;
		EIMSK |= (1 << INT0);
		EICRA |= (1 << ISC01);
	} else {
		e1 = this;
		EIMSK |= (1 << INT1);
		EICRA |= (1 << ISC11);
	}
	pinMode(_pin, INPUT);
	digitalWrite(_pin, HIGH);	// enable pullup
}

