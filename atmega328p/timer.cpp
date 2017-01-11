#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "timer.h"

// Timer 1 is used so as to avoid conflict with the timer 0, used by the delay() function in the Arduino library
static Device *t1;
static uint16_t count_adjust = 0;  // An adjustment amount for the comparison value (alternates between 0 and 1)

// Ensure that these are defined properly
#define power_timer1_enable()   (PRR &= (uint8_t)~(1 << PRTIM1))
#define power_timer1_disable()  (PRR |= (uint8_t)(1 << PRTIM1))

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
	wake();

	TCCR1A = 0;
	// CTC mode, 1024 prescaler
	TCCR1B = bit(WGM12) | bit(CS10) | bit(CS12);

	OCR1A = (uint16_t) (F_CPU / (1024000L * (uint32_t) _ms_divisor)) - 1;	// 1 ms/_ms_divisor approximately
	sleep();
	return false;
}

void Timer::_enable(bool e) {
	uint8_t saved_SREG = SREG;	// Save the interrupt flag
	cli();					// disable interrupts while resetting the timer/counter
	TCNT1 = 0;				// Reset the timer/counter
	SREG = saved_SREG;		// restore the interrupt flag

	bitWrite(TIMSK1, OCIE1A, e);
}

// Alternate the value of the timer compare value to increase accuracy
// void Timer::ready() {
// 	count_adjust = 1 - count_adjust;
// 	uint16_t new_compare_value = F_CPU / (1024000L*_ms_divisor) - count_adjust;
// 	uint8_t saved_SREG = SREG;	// Save the interrupt flag
// 	cli();				// disable interrupts while changing the compare count
// 	OCR1A = new_compare_value;
// 	SREG = saved_SREG;	// restore the interrupt flag
//
// 	AbstractTimer::ready();
// }

unsigned Timer::_sleepmode() {
	return SLEEP_MODE_IDLE;
}
