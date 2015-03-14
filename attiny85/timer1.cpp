#include <Arduino.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "device.h"
#include "timer.h"
#include "timer1.h"

static Device *t1;

ISR(TIM1_COMPA_vect) {
	if (t1)
		t1->ready();
}

// FIXME: other clock frequencies?
bool Timer1::begin() {
	t1 = this;

#if F_CPU == 8000000L
	// CTC mode, divide by 32
	TCCR1 = _BV(CTC1) | _BV(CS12) | _BV(CS11);
	OCR1C = F_CPU / 32000 - 1;
#elif F_CPU == 1000000L
	// CTC mode, divide by 4
	TCCR1 = _BV(CTC1) | _BV(CS11) | _BV(CS10);
	OCR1C = F_CPU / 8000 - 1;
#endif
	return false;
}

void Timer1::_enable(bool e) {
	if (e)
		TIMSK |= _BV(OCIE1A);
	else
		TIMSK &= ~_BV(OCIE1A);
}

unsigned Timer1::sleepmode() {
	return SLEEP_MODE_IDLE;
}
