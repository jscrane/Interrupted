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
void External::begin() {
	e0 = this;
	MCUCR &= ~(bit(ISC01) | bit(ISC00));
	if (_mode == CHANGE)
		MCUCR |= bit(ISC00);
	else if (_mode == FALLING)
		MCUCR |= bit(ISC01);
	else if (_mode == RISING)
		MCUCR |= bit(ISC01) | bit(ISC00);

	pinMode(_pin, INPUT);
	digitalWrite(_pin, HIGH);	// enable pullup
}

void External::_enable(bool e) {
	if (e)
		GIMSK |= bit(INT0);
	else
		GIMSK &= ~bit(INT0);
}
