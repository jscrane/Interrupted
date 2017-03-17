#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "device.h"
#include "timer.h"

// Timer 1 is used so as to avoid conflict with the timer 0, used by the delay() function in the Arduino library
static Device *t1;

// Ensure that these are defined properly
#define power_timer1_enable()   (PRR &= (uint8_t)~(1 << PRTIM1))
#define power_timer1_disable()  (PRR |= (uint8_t)(1 << PRTIM1))

ISR(TIM1_COMPA_vect) {
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
	wake();	 // Timer must be powered on for the register changes to take effect

	TCCR1A = 0;
	// CTC mode, clock stopped
	TCCR1B = _BV(WGM12);

	uint8_t saved_SREG = SREG;	// Save the interrupt flag
	cli();					// disable interrupts while resetting the timer/counter
	OCR1A = (uint16_t) (F_CPU / (1000 * (uint32_t) _ms_divisor)) - 1;	// 1ms divided by the divisor
	SREG = saved_SREG;		// restore the interrupt flag
	sleep();

	return false;
}

// Timer must be awake before calling this function
void Timer::_enable(bool e) {
	uint8_t sreg = SREG;
	cli();
	TCNT1 = 0;
  bitWrite(TCCR1B, CS10, e);		// Start or stop the clock
	if (e)
		TIMSK1 |= _BV(OCIE1A);
	else
		TIMSK1 &= ~_BV(OCIE1A);
  SREG = sreg;
}

unsigned Timer::_sleepmode() {
	return SLEEP_MODE_IDLE;
}
