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

bool Timer1::begin() {
	t1 = this;

	TCCR1A = 0;
	// CTC mode, no prescaler
	TCCR1B = _BV(WGM12) | _BV(CS10);

	OCR1A = F_CPU / 1000 - 1;	// 1ms
	return false;
}

void Timer1::_enable(bool e) {
	if (e)
		TIMSK1 |= _BV(OCIE1A);
	else
		TIMSK1 &= ~_BV(OCIE1A);
}

unsigned Timer1::sleepmode() {
	return SLEEP_MODE_IDLE;
}

