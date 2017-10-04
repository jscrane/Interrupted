#include <Energia.h>
#include <stdarg.h>

#include "device.h"

void Devices::begin() {
	for (int i = 0; i < _n; i++)
		_devices[i]->enable(_devices[i]->begin());
}

unsigned Device::_sleepmode() {
	return LPM4_bits;
}

unsigned Devices::compare_modes(unsigned sys, unsigned dev) {
	switch (dev) {
	case LPM0_bits:
		return LPM0_bits;
	case LPM1_bits:
		if (sys != LPM0_bits)
			return LPM1_bits;
		break;
	case LPM2_bits:
		if (sys != LPM0_bits && sys != LPM1_bits)
			return LPM2_bits;
		break;
	case LPM3_bits:
		if (sys == LPM4_bits)
			return LPM3_bits;
		break;
	case LPM4_bits:
		break;
	}
	return sys;
}

void Devices::sleep(unsigned mode) {
	_BIS_SR(mode | GIE);
}

int Devices::select() {
	noInterrupts();
	unsigned mode = LPM4_bits;
	for (int i = 0; i < _n; i++) {
		Device *d = _devices[i];
		if (d->is_ready()) {
			interrupts();
			return d->id();
		}
		if (d->is_enabled())
			mode = d->negotiate_mode(mode);
	}
	idle(mode);
	return -1;
}

void Device::_no_ints() {
	noInterrupts();
}

void Device::_ints() {
	interrupts();
}
