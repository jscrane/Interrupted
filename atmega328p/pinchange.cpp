#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static PinChangeGroup *d[3];

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

static int port_bit(byte port, int pin) {
	if (port == PD)
		return pin;
	if (port == PB)
		return pin - 8;
	if (port == PC)
		return pin - 14;

	return -1;
}

void PinChangeGroup::add_pin(int pin, PinChange *p) {
	_pins[port_bit(_port, pin)] = p;
	byte b = digitalPinToBitMask(pin);
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

void PinChangeGroup::enable_pin(int pin, bool enable) {
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

void PinChangeGroup::ready() {
	int off = -port_bit(_port, 0);
	for (int i = 0; i < 8; i++) {
		byte b = bit(i);
		if (_pins[i] && (_enabled & b)) {
			int v = digitalRead(i + off);
			if (v != (_state & b)) {
				if (v)
					_state |= b;
				else
					_state &= ~b;
				_pins[i]->set_state(v != 0);
			}
		}
	}
}
