#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

#include <Arduino.h>
#include "atomic.h"
#include "device.h"
#include "watchdog.h"

static Device *wdt;

ISR(WDT_vect)
{
	if (wdt)
		wdt->ready();
}

bool Watchdog::begin() {
	wdt = this;
	MCUSR &= ~_BV(WDRF);
	return false;
}

void Watchdog::_enable(bool e) {
	if (e) {
		wdt_reset();
		_left = _update_prescaler(_secs);
	} else {
		Atomic block;
		WDTCR = _BV(WDCE) | _BV(WDE);
		WDTCR = 0;
	}
}

unsigned Watchdog::_update_prescaler(unsigned t) {
	uint8_t b = _BV(WDIE);
	if (t >= 8) {
		b |= _BV(WDP3) | _BV(WDP0);
		t -= 8;
	} else if (t >= 4) {
		b |= _BV(WDP3);
		t -= 4;
	} else if (t >= 2) {
		b |= _BV(WDP2) | _BV(WDP1) | _BV(WDP0);
		t -= 2;
	} else {
		b |= _BV(WDP2) | _BV(WDP1);
		t = 0;
	}
	Atomic block;
	WDTCR = _BV(WDCE) | _BV(WDE);
	WDTCR = b;
	return t;
}

unsigned Watchdog::_sleepmode() {
	return SLEEP_MODE_PWR_DOWN;
}
