#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static Port *d;

// handler for PB
ISR(PCINT0_vect) {
	if (d)
		d->ready();
}

void Port::add_pin(int pin, Pin *p) {
	if (!d)
		d = this;
	_pins[pin] = p;
	PCMSK |= bit(pin);
}

void Port::enable_pin(int pin, bool enable) {
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

void Port::ready() {
	for (int i = 0; i < 8; i++) {
		byte b = bit(i);
		if (_pins[i] && (_enabled & b)) {
			bool hi = digitalRead(i) == HIGH;
			if (hi != _pins[i]->is_high())
				_pins[i]->set_state(hi);
		}
	}
}
