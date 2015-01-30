#include <avr/power.h>
#include <avr/sleep.h>

#include <Arduino.h>
#include <stdarg.h>

#include "device.h"

void Devices::begin() {
	// turn off ADC and analog comparator
	ADCSRA &= ~bit(ADEN);
	ACSR &= ~bit(ACD);
	power_adc_disable();

	for (int i = 2; i <= A5; i++) {
		pinMode(i, INPUT);
		digitalWrite(i, LOW);
	}

	for (int i = 0; i < _n; i++) {
		_devices[i]->begin();
		_devices[i]->enable();
	}

	sei();
}

bool Device::is_ready() {
	if (_enabled && _ready) {
		_ready = false;
		return true;
	}
	return false;
}

int Devices::select() {
again:
	// so we don't miss an interrupt while checking...
	cli();
	for (int i = 0; i < _n; i++)
		if (_devices[i]->is_ready()) {
			sei();
			return _devices[i]->id();
		}

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();

	// this exists on later avrlibs but not the one shipped with
	// arduino. also the later arduino core won't compile under
	// the latest avr-gcc so we're stuck with arduino10!
	sleep_bod_disable();
	sei();
	sleep_cpu();
	sleep_disable();

	goto again;
}
