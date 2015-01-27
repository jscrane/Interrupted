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

	for (int i = 0; i < _n; i++)
		_devices[i]->begin();

	sei();
}

bool Device::ready() {
	if (_enabled) {
		bool t = _triggered;
		_triggered = false;
		return t;
	}
	return false;
}

int Devices::select() {

	// so we don't miss an interrupt while checking...
	cli();
	for (int i = 0; i < _n; i++)
		if (_devices[i]->ready()) {
			sei();
			return _devices[i]->id();
		}

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sei();
	sleep_cpu();                   // go to sleep
	sleep_disable();

	for (int i = 0; i < _n; i++)
		if (_devices[i]->ready())
			return _devices[i]->id();

	// we've been woken up but the device is either not managed by
	// this device-set or it's not enabled / ready
	return -1;
}
