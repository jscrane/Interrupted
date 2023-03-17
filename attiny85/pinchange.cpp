#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static Pin *pins[8];
static uint8_t enabled;

// handler for PB
ISR(PCINT0_vect) {
	Ports::ready(0);
}

void Ports::register_pin(uint8_t pin, Pin *p) {
	pins[pin] = p;
	PCMSK |= bit(pin);
}

void Ports::enable_pin(uint8_t pin, bool enable) {
	uint8_t b = bit(pin);
	uint8_t prev = enabled;
	if (enable) {
		enabled |= b;
		if (!prev)
			GIMSK |= bit(PCIE);
	} else {
		enabled &= ~b;
		if (prev && !enabled)
			GIMSK &= ~bit(PCIE);
	}
}

void Ports::ready(uint8_t) {
	for (int i = 0; i < 8; i++)
		if (pins[i] && (enabled & bit(i)))
			pins[i]->set_state(digitalRead(i));
}
