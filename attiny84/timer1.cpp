#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

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
	// CTC mode, 1024 prescaler
	TCCR1B = bit(WGM12) | bit(CS10) | bit(CS12);

	OCR1A = F_CPU / 1024000 - 1;	// 1 ms
	return false;
}

void Timer1::_enable(bool e) {
	if (e)
		TIMSK1 |= bit(OCIE1A);
	else
		TIMSK1 &= ~bit(OCIE1A);
}

unsigned Timer1::sleepmode() {
	return SLEEP_MODE_IDLE;
}

