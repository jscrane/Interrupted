#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include <Arduino.h>
#include <stdarg.h>

#include "device.h"

// Ensure that these are defined properly (there appears to be a bug in avr/power.h for ATMega328[P])
#define power_all_enable()      (PRR &= (uint8_t)~((1<<PRADC)|(1<<PRTIM0)|(1<<PRTIM1)|(1<<PRTIM2)|(1<<PRSPI)|(1<<PRTWI)|(1<<PRUSART0)))
#define power_all_disable()     (PRR |= (uint8_t)((1<<PRADC)|(1<<PRTIM0)|(1<<PRTIM1)|(1<<PRTIM2)|(1<<PRSPI)|(1<<PRTWI)|(1<<PRUSART0)))

void Devices::begin() {
	// "...[it] is therefore required to turn off the watchdog
	// early during program startup..." (from avr/wdt.h)
	wdt_disable();

	// turn off ADC and analog comparator
	ADCSRA &= ~bit(ADEN);
	ACSR |= bit(ACD);

	// power down all modules until they are needed
	power_all_disable();

	// turn off the brown-out detector
	MCUCR |= _BV(BODS) | _BV(BODSE);

	for (int i = 2; i <= A5; i++)
		digitalWrite(i, LOW);

	for (int i = 0; i < _n; i++)
	{
		if (_devices[i]->begin())
		{
			_devices[i]->wake();
			_devices[i]->enable(true);
		}
	}

	sei();
}

unsigned Device::_sleepmode() {
	return SLEEP_MODE_PWR_DOWN;
}

// required because there's no defined ordering of modes...
unsigned Devices::compare_modes(unsigned sys, unsigned dev) {
	switch (dev) {
	case SLEEP_MODE_IDLE:
		return SLEEP_MODE_IDLE;
	case SLEEP_MODE_ADC:
		if (sys != SLEEP_MODE_IDLE)
			return SLEEP_MODE_ADC;
		break;
	case SLEEP_MODE_PWR_SAVE:
		if (sys != SLEEP_MODE_IDLE && sys != SLEEP_MODE_ADC)
			return SLEEP_MODE_PWR_SAVE;
		break;
	case SLEEP_MODE_EXT_STANDBY:
		if (sys == SLEEP_MODE_PWR_DOWN || sys == SLEEP_MODE_STANDBY)
			return SLEEP_MODE_EXT_STANDBY;
		break;
	case SLEEP_MODE_STANDBY:
		if (sys == SLEEP_MODE_PWR_DOWN)
			return SLEEP_MODE_STANDBY;
		break;
	case SLEEP_MODE_PWR_DOWN:
		break;
	}
	return sys;
}

void Devices::sleep(unsigned mode) {
	set_sleep_mode(mode);
	sleep_enable();

	sleep_bod_disable();
	sei();
	sleep_cpu();
	sleep_disable();
}

int Devices::select() {
	// so we don't miss an interrupt while checking...
	cli();
	unsigned mode = SLEEP_MODE_PWR_DOWN;
	for (int i = 0; i < _n; i++) {
		Device *d = _devices[i];
		if (d->is_ready()) {
			sei();
			return d->id();
		}
		if (d->is_enabled())
			mode = d->negotiate_mode(mode);
	}
	idle(mode);
	return -1;
}
