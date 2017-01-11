#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

#include "device.h"
#include "atimer.h"
#include "watchdog.h"

static Device *wdt;

ISR(WDT_vect)
{
	if (wdt)
		wdt->ready();
}

// http://donalmorrissey.blogspot.ie/2010/04/sleeping-arduino-part-5-wake-up-via.html
void Watchdog::_prescale() {
	unsigned prescale = 0;
	switch (_scale) {
	case WDTO_15MS:  // Actually 16 ms
		break;
	case WDTO_30MS:
		prescale = _BV(WDP0);  // Actually 32 ms
		break;
	case WDTO_60MS:
		prescale = _BV(WDP1);  // Actually 64 ms
		break;
	case WDTO_120MS:
		prescale = _BV(WDP1) |  _BV(WDP0);  // Actually 125 ms
		break;
	case WDTO_250MS:
		prescale = _BV(WDP2);
		break;
	case WDTO_500MS:
		prescale = _BV(WDP2) |  _BV(WDP0);
		break;
	case -1:
	case WDTO_1S:
		prescale = _BV(WDP2) | _BV(WDP1);
		break;
	case WDTO_2S:
		prescale = _BV(WDP2) | _BV(WDP1) | _BV(WDP0);
		break;
	case WDTO_4S:
		prescale = _BV(WDP3);
		break;
	case WDTO_8S:
		prescale = _BV(WDP3) | _BV(WDP0);
		break;
	}

	cli();
	MCUSR &= ~_BV(WDRF);
	WDTCSR |= _BV(WDCE) | _BV(WDE) | _BV(WDIF);
	WDTCSR = prescale;	// change prescaler, also setting WDE bit to 0
	sei();
}

bool Watchdog::begin() {
	wdt = this;
	_prescale();
	return false;
}

void Watchdog::_enable(bool e) {
	wdt_reset();		// Ensure that the timer will start from 0 again
	if (e)
		WDTCSR |= _BV(WDIE);	// Enable the interrupt without changing the prescale value
	else
		WDTCSR &= ~_BV(WDIE);   // Since WDE is zero, the timer is stopped
}

unsigned Watchdog::_sleepmode() {
	return SLEEP_MODE_PWR_DOWN;
}
