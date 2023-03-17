#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static Pin *pins[3][8];
static uint8_t enabled[3];

// handler for PB
ISR(PCINT0_vect) {
	Ports::ready(0);
}

// handler for PC
ISR(PCINT1_vect) {
	Ports::ready(1);
}

// handler for PD
ISR(PCINT2_vect) {
	Ports::ready(2);
}

void Ports::register_pin(uint8_t pin, Pin *p) {
	uint8_t b = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin) - 2;
	pins[port][bit_index(b)] = p;
	if (port == 0)
		PCMSK0 |= b;
	else if (port == 1)
		PCMSK1 |= b;
	else if (port == 2)
		PCMSK2 |= b;
}

void Ports::enable_pin(uint8_t pin, bool enable) {
	uint8_t e = 0, f = 0;
	uint8_t port = digitalPinToPort(pin) - 2;
	switch (port) {
	case 0:
		e = bit(PCIE0);
		f = bit(PCIF0);
		break;
	case 1:
		e = bit(PCIE1);
		f = bit(PCIF1);
		break;
	case 2:
		e = bit(PCIE2);
		f = bit(PCIF2);
		break;
	default:
		return;
	}

	uint8_t b = digitalPinToBitMask(pin);
	uint8_t &enabled = ::enabled[port];
	if (enable) {
		if (!enabled) {
			PCIFR |= f;
			PCICR |= e;
		}
		enabled |= b;
	} else {
		uint8_t prev = enabled;
		enabled &= ~b;
		if (prev && !enabled) {
			PCIFR &= ~f;
			PCICR &= ~e;
		}
	}
}

void Ports::ready(uint8_t port) {
	uint8_t v = (*portInputRegister(port + 2));
	Pin **pins = ::pins[port];
	uint8_t enabled = ::enabled[port];
	for (int i = 0; i < 8; i++) {
		uint8_t b = bit(i);
		if (pins[i] && (enabled & b))
			pins[i]->set_state(v & b);
	}
}
