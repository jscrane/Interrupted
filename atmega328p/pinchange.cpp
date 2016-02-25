#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static Port *d[3];

// ugh (these are invisible in Arduino.h)
#define PA 1
#define PB 2
#define PC 3
#define PD 4

// handler for PB
ISR(PCINT0_vect) {
	if (d[0])
		d[0]->ready();
}

// handler for PC
ISR(PCINT1_vect) {
	if (d[1])
		d[1]->ready();
}

// handler for PD
ISR(PCINT2_vect) {
	if (d[2])
		d[2]->ready();
}

void Port::add_pin(int pin, Pin *p) {
	if (!_port)
		_port = digitalPinToPort(pin);
	byte b = digitalPinToBitMask(pin);
	_pins[bit_index(b)] = p;
	if (_port == PB) {
		PCMSK0 |= b;
		d[0] = this;
	} else if (_port == PC) {
		PCMSK1 |= b;
		d[1] = this;
	} else if (_port == PD) {
		PCMSK2 |= b;
		d[2] = this;
	}
}

void Port::enable_pin(int pin, bool enable) {
	byte e = 0, f = 0;
	switch (_port) {
	case PA:
		return;		// error
	case PB:
		e = bit(PCIE0);
		f = bit(PCIF0);
		break;
	case PC: 
		e = bit(PCIE1);
		f = bit(PCIF1);
		break;
	case PD:
		e = bit(PCIE2);
		f = bit(PCIF2);
		break;
	}
	byte b = digitalPinToBitMask(pin);
	byte prev = _enabled;
	if (enable) {
		_enabled |= b;
		if (!prev) {
			PCIFR |= f;
			PCICR |= e;
		}
	} else {
		_enabled &= ~b;
		if (prev && !_enabled) {
			PCIFR &= ~f;
			PCICR &= ~e;
		}
	}
}

void Port::ready() {
	byte v = (*portInputRegister(_port));
	for (int i = 0; i < 8; i++) {
		byte b = bit(i);
		bool hi = (v & b);
		if (_pins[i] && (_enabled & b) && hi != _pins[i]->is_high())
			_pins[i]->set_state(hi);
	}
}
