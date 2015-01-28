#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <Arduino.h>

#include "device.h"
#include "timer.h"

void Timer::trigger() {
	if (--_curr == 0) {
		Device::trigger();
		_curr = _secs;
	}
}

static Device *wdt;

ISR(WDT_vect) 
{
	if (wdt)
		wdt->trigger();
}

// http://donalmorrissey.blogspot.ie/2010/04/sleeping-arduino-part-5-wake-up-via.html
void Watchdog::begin() {
	wdt = this;

	MCUSR &= ~bit(WDRF);
	WDTCSR |= bit(WDCE) | bit(WDE);			// change prescaler
	WDTCSR = bit(WDP2) | bit(WDP1) | bit(WDIE);	// 1s, interrupt mode
}

/* FIXME: need to prevent going into SLEEP_MODE_POWER_DOWN
void Timer1::begin() {
	TCCR1A = 0;
	// CTC mode, 1024 prescaler
	TCCR1B = bit(WGM12) | bit(CS10) | bit(CS12);

	OCR1A = F_CPU / 1024 - 1;	// 1 sec
	TIMSK1 = bit(OCIE1A);
*/
