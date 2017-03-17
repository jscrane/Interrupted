#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "device.h"
#include "timer.h"

// Timer 1 is used so as to avoid conflict with the timer 0, used by the delay() function in the Arduino library
static Device *t1;

// Ensure that these are defined properly
#ifndef power_timer1_enable
#define power_timer1_enable()   (PRR &= (uint8_t)~(1 << PRTIM1))
#endif
#ifndef power_timer1_disable
#define power_timer1_disable()  (PRR |= (uint8_t)(1 << PRTIM1))
#endif

ISR(TIMER1_COMPA_vect) {
	if (t1)
		t1->ready();
}

// call to turn off device altogether
void Timer::sleep()
{
	power_timer1_disable();
}

// call to turn device on again
void Timer::wake()
{
	power_timer1_enable();
}


bool Timer::begin() {
	t1 = this;

	power_timer1_enable();

	TCCR1A = 0;
	// CTC mode, 1024 prescaler
	TCCR1B = _BV(WGM12) | _BV(CS10) | _BV(CS12);

	OCR1A = (uint16_t) (F_CPU / 1024000L) - 1; // approximately 1 ms divided by the divisor
	sleep();
	return false;
}

void Timer::_enable(bool e) {
	uint8_t sreg = SREG;
	cli();
	TCNT1 = 0;
	if (e)
		TIMSK1 |= _BV(OCIE1A);
	else
		TIMSK1 &= ~_BV(OCIE1A);
	SREG = sreg;

}

unsigned Timer::_sleepmode() {
	return SLEEP_MODE_IDLE;
}
