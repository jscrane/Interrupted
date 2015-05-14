#include <intrinsics.h>
#include <Energia.h>
#include <stdarg.h>

#include "device.h"

void Devices::begin() {
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
/*
	for (int i = 2; i < 20; i++)
		digitalWrite(i, LOW);
*/

	for (int i = 0; i < _n; i++)
		_devices[i]->enable(_devices[i]->begin());

	__eint();
}

unsigned Device::sleepmode() {
	return LPM0_bits;
}

static unsigned update_mode(unsigned m, unsigned mode) {
	switch (mode) {
	case LPM0_bits:
		return mode;
	case LPM1_bits:
		if (m != LPM0_bits)
			return mode;
		break;
	case LPM2_bits:
		if (m != LPM0_bits && m != LPM1_bits)
			return mode;
		break;
	case LPM3_bits:
		if (m == LPM4_bits)
			return mode;
		break;
	case LPM4_bits:
		break;
	}
	return m;
}

int Devices::select() {
again:
	noInterrupts();
	unsigned mode = LPM0_bits;
	for (int i = 0; i < _n; i++) {
		Device *d = _devices[i];
		if (d->is_ready()) {
			interrupts();
			return d->id();
		}
		if (d->is_enabled())
			mode = update_mode(mode, d->sleepmode());
	}
	_BIS_SR(mode | GIE);
	goto again;
}
