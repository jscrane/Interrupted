#include <avr/power.h>
#include <avr/sleep.h>

#include <Arduino.h>
#include <stdarg.h>

#include "device.h"

void Devices::begin() {
	for (int i = 2; i < 14; i++) {
		pinMode(i, INPUT);
		digitalWrite(i, HIGH);
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
	sleep_bod_disable();
	sei();
	sleep_cpu();
	sleep_disable();

	goto again;
}
