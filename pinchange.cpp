#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static PinChangeGroup *d[3];

// handler for D8-D13
ISR(PCINT0_vect) {
	if (d[0])
		d[0]->ready();
}

ISR(PCINT1_vect) {
	if (d[1])
		d[1]->ready();
}

ISR(PCINT2_vect) {
	if (d[2])
		d[2]->ready();
}

void PinChangeGroup::add_pin(PinChangePin *p, int pin) {
	d[_group] = this;
	int o = pin - port_offset();
	byte b = bit(o);
	_pins[o] = p;
	if (_group == D0_7)
		PCMSK2 |= b;
	else if (_group == D8_13)
		PCMSK0 |= b;
	else if (_group == A0_5)
		PCMSK1 |= b;
}

void PinChangeGroup::enable_pin(int pin, boolean en) {
	byte e = 0, f = 0;
	switch (_group) {
	case D8_13:
		e = bit(PCIE0);
		f = bit(PCIF0);
		break;
	case A0_5:
		e = bit(PCIE1);
		f = bit(PCIF1);
		break;
	case D0_7:
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
	if (_group == D0_7)
		return 0;
	if (_group == D8_13)
		return 8;
	if (_group == A0_5)
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

void PinChangePin::begin() {
	_group.add_pin(this, id());
}

void PinChangePin::enable(bool enable) {
	Device::enable(enable);
	_group.enable_pin(id(), enable);
}
