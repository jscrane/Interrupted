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
	unsigned sreg = SREG;
	cli();
	uint8_t b = 0;
	if (e) {
		wdt_reset();  // Ensure that the timer will start from 0 again
		b = _BV(WDIE) | _scale;
	}
	WDTCSR = _BV(WDCE) | _BV(WDE);
	WDTCSR = b;
	SREG = sreg;
}

unsigned Watchdog::_sleepmode() {
	return SLEEP_MODE_PWR_DOWN;
}
