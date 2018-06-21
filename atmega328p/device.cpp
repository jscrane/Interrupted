#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "atomic.h"
#include "device.h"

void Devices::begin() {
	// "...[it] is therefore required to turn off the watchdog
	// early during program startup..." (from avr/wdt.h)
	Atomic block;
	wdt_disable();

	// turn off ADC and analog comparator
	ADCSRA &= ~bit(ADEN);
	ACSR |= bit(ACD);
	power_all_disable();

	// turn off the brown-out detector
	MCUCR |= _BV(BODS) | _BV(BODSE);

	for (int i = 2; i <= A5; i++)
		digitalWrite(i, LOW);

	for (int i = 0; i < _n; i++)
		_devices[i]->enable(_devices[i]->begin());
}

unsigned Device::_sleepmode() {
	return SLEEP_MODE_PWR_DOWN;
}

// SLEEP_MODE_IDLE		0	(delay, serial, timer)
// SLEEP_MODE_ADC		2	(analog)
// SLEEP_MODE_PWR_DOWN		4	(external, pinchange, watchdog)
// SLEEP_MODE_PWR_SAVE		6
// SLEEP_MODE_STANDBY		12
// SLEEP_MODE_EXT_STANDBY	14

unsigned Devices::compare_modes(unsigned sys, unsigned dev) {
	switch (sys) {
	case SLEEP_MODE_NONE:
	case SLEEP_MODE_PWR_DOWN:
		return dev;

	case SLEEP_MODE_IDLE:
		return SLEEP_MODE_IDLE;

	case SLEEP_MODE_ADC:
		if (dev == SLEEP_MODE_IDLE)
			return SLEEP_MODE_IDLE;
		return SLEEP_MODE_ADC;

	case SLEEP_MODE_PWR_SAVE:
		if (dev == SLEEP_MODE_IDLE || dev == SLEEP_MODE_ADC)
			return dev;
		return SLEEP_MODE_PWR_SAVE;

	case SLEEP_MODE_EXT_STANDBY:
		if (dev == SLEEP_MODE_STANDBY || dev == SLEEP_MODE_PWR_DOWN)
			return SLEEP_MODE_EXT_STANDBY;
		return dev;

	case SLEEP_MODE_STANDBY:
		if (dev == SLEEP_MODE_PWR_DOWN)
			return SLEEP_MODE_STANDBY;
		return dev;
	}
	return sys;
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

int Devices::select() {
	// so we don't miss an interrupt while checking...
	Atomic block;
	unsigned mode = SLEEP_MODE_NONE;
	for (int i = 0; i < _n; i++) {
		Device *d = _devices[i];
		if (d->is_ready())
			return d->id();
		if (d->is_enabled())
			mode = d->negotiate_mode(mode);
	}
	idle(mode);
	return -1;
}
