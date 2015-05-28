#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static PinChangeGroup *d[2];

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

void PinChangeGroup::add_pin(int pin, PinChange *p) {
	if (!_port)
		_port = digitalPinToPort(pin);
	byte b = digitalPinToBitMask(pin);
	_pins[bit_index(b)] = p;
	if (_port == PORTA) { 
		PCMSK0 |= b; 
		d[0] = this;
	} else if (_port == PORTB) {
		PCMSK1 |= b;
		d[1] = this;
	}
}

void PinChangeGroup::enable_pin(int pin, bool enable) {
	byte e = 0;
	if (_port == PORTA)
		e = bit(PCIE0);
	else if (_port == PORTB)
		e = bit(PCIE1);
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

void PinChangeGroup::ready() {
	uint8_t v = (*portInputRegister(_port));
	for (int i = 0; i < 8; i++) {
		byte b = bit(i);
		if (_pins[i] && (_enabled & b) && (v & b) != (_state & b)) {
			_state ^= b;
			_pins[i]->set_state(v != 0);
		}
	}
}
