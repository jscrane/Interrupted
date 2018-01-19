#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "atomic.h"
#include "device.h"
#include "timer.h"

static Device *t1;

ISR(TIMER1_COMPA_vect) {
	if (t1)
		t1->ready();
}

bool Timer::begin() {
	t1 = this;
	power_timer1_enable();

	TCCR1A = 0;
	// CTC mode, 1024 prescaler
	TCCR1B = _BV(WGM12) | _BV(CS10) | _BV(CS12);

	OCR1A = F_CPU / 1024000 - 1;	// 1 ms
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

