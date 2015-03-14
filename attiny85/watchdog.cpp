#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "device.h"
#include "timer.h"
#include "watchdog.h"

static Device *wdt;

ISR(WDT_vect) 
{
	if (wdt)
		wdt->ready();
}

// http://donalmorrissey.blogspot.ie/2010/04/sleeping-arduino-part-5-wake-up-via.html
bool Watchdog::begin() {
	wdt = this;

	unsigned prescale = 0;
	switch (_scale) {
	case WDTO_15MS:
		break;
	case WDTO_30MS:
		prescale = _BV(WDP0);
		break;
	case WDTO_60MS:
		prescale = _BV(WDP1);
		break;
	case WDTO_120MS:
		prescale = _BV(WDP1) |  _BV(WDP0);
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
	WDTCR |= _BV(WDCE) | _BV(WDE) | _BV(WDIF);	// change prescaler
	WDTCR = prescale;
	sei();
	return false;
}

void Watchdog::_enable(bool e) {
	if (e)
		WDTCR |= _BV(WDIE);
	else
		WDTCR &= ~_BV(WDIE);
}
