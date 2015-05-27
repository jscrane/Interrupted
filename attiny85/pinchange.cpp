#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static PinChangeGroup *d;

// handler for PB
ISR(PCINT0_vect) {
	if (d)
		d->ready();
}

void PinChangeGroup::add_pin(int pin, PinChange *p) {
	if (!d)
		d = this;
	_pins[pin] = p;
	PCMSK |= bit(pin);
}

void PinChangeGroup::enable_pin(int pin, bool enable) {
	byte b = bit(pin);
	byte prev = _enabled;
	if (enable) {
		_enabled |= b;
		if (!prev)
			GIMSK |= bit(PCIE);
	} else {
		_enabled &= ~b;
		if (prev && !_enabled)
			GIMSK &= ~bit(PCIE);
	}
}

void PinChangeGroup::ready() {
	for (int i = 0; i < 8; i++) {
		byte b = bit(i);
		if (_pins[i] && (_enabled & b)) {
			int v = digitalRead(i);
			if (v != (_state & b)) {
				_state ^= b;
				_pins[i]->set_state(v != 0);
			}
		}
	}
}
