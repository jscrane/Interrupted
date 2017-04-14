#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

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
	if (_scale == -1)
		_scale = WDTO_1S;
	MCUSR &= ~_BV(WDRF);
	return false;
}

void Watchdog::_enable(bool e) {
	wdt_reset();		// Ensure that the timer will start from 0 again
	cli();
	uint8_t b = e? _BV(WDIE) | _scale: 0;
	WDTCSR = _BV(WDCE) | _BV(WDE);
	WDTCSR = b;
	sei();
}

unsigned Watchdog::_sleepmode() {
	return SLEEP_MODE_PWR_DOWN;
}
