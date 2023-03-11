#include <Arduino.h>
#include <stdarg.h>

#include "device.h"

void Devices::begin(bool powersave) {
	enable();
}

unsigned Device::_sleepmode() {
	return LPM4_bits;
}

unsigned Devices::compare_modes(unsigned sys, unsigned dev) {
	if (sys == SLEEP_MODE_NONE)
		return dev;
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
	if (mode != SLEEP_MODE_NONE)
		_BIS_SR(mode | GIE);
}
