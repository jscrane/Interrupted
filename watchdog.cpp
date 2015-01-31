#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <Arduino.h>

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
void Watchdog::begin() {
	wdt = this;

	MCUSR &= ~bit(WDRF);
	WDTCSR |= bit(WDCE) | bit(WDE);		// change prescaler
	WDTCSR = bit(WDP2) | bit(WDP1);		// 1s
}

void Watchdog::enable(bool e) {
	if (e)
		WDTCSR |= bit(WDIE);
	else
		WDTCSR &= ~bit(WDIE);
}

