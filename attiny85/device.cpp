#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include <Arduino.h>

#include "device.h"

unsigned Device::_sleepmode() {
	return SLEEP_MODE_PWR_DOWN;
}

void Devices::begin() {
	// "...[it] is therefore required to turn off the watchdog
	// early during program startup..." (from avr/wdt.h)
	cli();
	wdt_disable();
	
	// turn off ADC and analog comparator
	ADCSRA = 0;
	ACSR |= bit(ACD);
	power_adc_disable();    // FIXME: power_all_disable()?
	
	// turn off the brown-out detector
	MCUCR |= _BV(BODS) | _BV(BODSE);
	
	for (int i = 0; i <= 10; i++)
	        digitalWrite(i, LOW);
	
	for (int i = 0; i < _n; i++)
	        _devices[i]->enable(_devices[i]->begin());
	
	sei();
}

unsigned Devices::compare_modes(unsigned sys, unsigned dev) {
	return dev < sys? dev: sys;
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
