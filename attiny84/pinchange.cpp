#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static Pin *pins[2][8];
static uint8_t enabled[2];

// handler for PA
ISR(PCINT0_vect) {
	Ports::ready(0);
}

// handler for PB
ISR(PCINT1_vect) {
	Ports::ready(1);
}

void Ports::register_pin(uint8_t pin, Pin *p) {
	uint8_t b = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin)-1;
	pins[port][bit_index(b)] = p;
	*(digitalPinToPCMSK(pin)) |= b;
}

void Ports::enable_pin(uint8_t pin, bool enable) {
	uint8_t e = bit(digitalPinToPCICRbit(pin));
	uint8_t b = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin)-1;
	uint8_t &enabled = ::enabled[port];
	uint8_t prev = enabled;
	if (enable) {
		enabled |= b;
		if (!prev)
			GIMSK |= e;
	} else {
		enabled &= ~b;
		if (prev && !enabled)
			GIMSK &= ~e;
	}
}

void Ports::ready(uint8_t port) {
	uint8_t v = (*portInputRegister(port+1));
	Pin **pins = ::pins[port];
	uint8_t enabled = ::enabled[port];
	for (int i = 0; i < 8; i++) {
		uint8_t b = bit(i);
		if (pins[i] && (enabled & b))
			pins[i]->set_state(v & b);
	}
}
