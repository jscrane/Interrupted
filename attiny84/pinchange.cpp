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

static int port_bit(pinchange_port_t port, int pin) {
	if (port == PB)
		return pin;
	if (port == PA)
		return 7 - (pin - 3);	// D3 == PA7, D9 == PA1
	return -1;
}

void PinChangeGroup::add_pin(int pin, PinChange *p) {
	int o = port_bit(_port, pin);
	byte b = bit(o);
	_pins[o] = p;
	if (_port == PA) {
		PCMSK0 |= b;
		d[0] = this;
	} else if (_port == PB) {
		PCMSK1 |= b;
		d[1] = this;
	}
}

void PinChangeGroup::enable_pin(int pin, boolean enable) {
	byte e = 0;
	if (_port == PA)
		e = bit(PCIE0);
	else if (_port == PB)
		e = bit(PCIE1);
	byte b = bit(port_bit(_port, pin));
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
	int off = 0;
	if (_port == PA)
		off = 3;
	for (int i = 0; i < 8; i++) {
		byte b = bit(i);
		if (_pins[i] && (_enabled & b)) {
			int v = digitalRead(off + i);
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
