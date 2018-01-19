#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "atomic.h"
#include "device.h"
#include "timer.h"

static Device *t1;

ISR(TIM1_COMPA_vect) {
	if (t1)
		t1->ready();
}

bool Timer::begin() {
	t1 = this;

	TCCR1A = 0;
	// CTC mode, no prescaler
	TCCR1B = _BV(WGM12) | _BV(CS10);

	OCR1A = F_CPU / 1000 - 1;	// 1ms
	return false;
}

void Timer::_enable(bool e) {
	Atomic block;
	TCNT1 = 0;
	if (e)
		TIMSK1 |= _BV(OCIE1A);
	else
		TIMSK1 &= ~_BV(OCIE1A);
}

unsigned Timer::_sleepmode() {
	return SLEEP_MODE_IDLE;
}
