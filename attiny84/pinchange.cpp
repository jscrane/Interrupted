#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static Port *d[2];

// handler for PA
ISR(PCINT0_vect) {
	if (d[0])
		d[0]->ready();
}

// handler for PB
ISR(PCINT1_vect) {
	if (d[1])
		d[1]->ready();
}

void Port::add_pin(int pin, Pin *p) {
	if (!_port)
		_port = digitalPinToPort(pin);
	byte b = digitalPinToBitMask(pin);
	_pins[bit_index(b)] = p;
	*(digitalPinToPCMSK(pin)) |= b;
	d[_port - 1] = this;
}

void Port::enable_pin(int pin, bool enable) {
	byte e = bit(digitalPinToPCICRbit(pin));
	byte b = digitalPinToBitMask(pin);
	byte prev = _enabled;
	if (enable) {
		_enabled |= b;
		if (!prev)
			GIMSK |= e;
	} else {
		_enabled &= ~b;
		if (prev && !_enabled)
			GIMSK &= ~e;
	}
}

void Port::ready() {
	uint8_t v = (*portInputRegister(_port));
	for (int i = 0; i < 8; i++) {
		byte b = bit(i);
		if (_pins[i] && (_enabled & b))
			_pins[i]->set_state(v & b);
	}
}
