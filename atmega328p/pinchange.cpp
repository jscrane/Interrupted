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

void PinChangeGroup::add_pin(PinChange *p, int pin) {
	int o = pin - port_offset();
	byte b = bit(o);
	_pins[o] = p;
	if (_group == PB) {
		PCMSK0 |= b;
		d[0] = this;
	} else if (_group == PC) {
		PCMSK1 |= b;
		d[1] = this;
	} else if (_group == PD) {
		PCMSK2 |= b;
		d[2] = this;
	}
}

void PinChangeGroup::enable_pin(int pin, boolean en) {
	byte e = 0, f = 0;
	switch (_group) {
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
	byte b = bit(pin - port_offset());
	byte prev = _enabled;
	if (en) {
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

int PinChangeGroup::port_offset() {
	if (_group == PD)
		return 0;
	if (_group == PB)
		return 8;
	if (_group == PC)
		return 14;

	return -1;
}

void PinChangeGroup::ready() {
	for (int i = 0; i < 8; i++) {
		byte b = bit(i);
		if (_pins[i] && (_enabled & b)) {
			int v = digitalRead(port_offset() + i);
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
