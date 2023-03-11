#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include <Arduino.h>

#include "atomic.h"
#include "device.h"

#define BODS 7		// BOD Sleep bit in MCUCR
#define BODSE 2		// BOD Sleep enable bit in MCUCR

void Devices::begin(bool powersave) {
	// "...[it] is therefore required to turn off the watchdog
	// early during program startup..." (from avr/wdt.h)
	Atomic block;

	if (powersave) {
		// turn off ADC and analog comparator
		ADCSRA = 0;
		ACSR |= bit(ACD);
		power_adc_disable();	// FIXME: power_all_disable()?

		// turn off the brown-out detector
		MCUCR |= _BV(BODS) | _BV(BODSE);

		for (int i = 0; i <= 10; i++)
			digitalWrite(i, LOW);
	}

	enable();
}

unsigned Device::_sleepmode() {
	return SLEEP_MODE_PWR_DOWN;
}

unsigned Devices::compare_modes(unsigned sys, unsigned dev) {
	if (sys == SLEEP_MODE_NONE)
		return dev;
	return dev < sys? dev: sys;
}

void Devices::sleep(unsigned mode) {
	if (mode != SLEEP_MODE_NONE) {
		set_sleep_mode(mode);
		sleep_enable();

		sleep_bod_disable();
		interrupts();
		sleep_cpu();
		sleep_disable();
	}
}
