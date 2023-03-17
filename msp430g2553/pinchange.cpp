#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static Pin *pins[2][8];
static uint8_t enabled[2];

#pragma vector=PORT1_VECTOR 
__interrupt void port1(void)
{
	Ports::ready(0);
	__bic_SR_register_on_exit(LPM4_bits | GIE);
}

#pragma vector=PORT2_VECTOR
__interrupt void port2(void)
{
	Ports::ready(1);
	__bic_SR_register_on_exit(LPM4_bits | GIE);
}

void Ports::ready(uint8_t port) {
	volatile uint8_t *ifg, *ies;
	uint8_t v = 0;
	if (port == 0) {
		ifg = &P1IFG;
		ies = &P1IES;
		v = P1IN;
	} else if (port == 1) {
		ifg = &P2IFG;
		ies = &P2IES;
		v = P2IN;
	} else
		return;

	Pin **pins = ::pins[port];
	uint8_t enabled = ::enabled[port];
	for (int i = 0; i < 8; i++) {
		uint8_t b = bit(i);
		// deal with the interrupt here whether the device
		// is enabled or not: ensure the next edge trigger
		// remains consistent with its current state
		// (i.e., don't just toggle it as in the examples).
		if (*ifg & b) {
			if (enabled & b)
				pins[i]->set_state((v & b) != 0);
			if (pins[i]->is_high())
				*ies |= b;	// next trigger on hi->lo
			else
				*ies &= ~b;	// next trigger on lo->hi
			*ifg &= ~b;
		}
	}
}

void Ports::register_pin(uint8_t pin, Pin *p) {
	uint8_t b = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	pins[port-1][bit_index(b)] = p;
}

void Ports::enable_pin(uint8_t pin, bool enable) {
	uint8_t b = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin)-1;
	uint8_t v = 0;
	volatile uint8_t *ie, *ifg, *ies;
	if (port == 0) {
		ie = &P1IE;
		ifg = &P1IFG;
		ies = &P1IES;
		v = P1IN;
	} else if (port == 1) {
		ie = &P2IE;
		ifg = &P2IFG;
		ies = &P2IES;
		v = P2IN;
	} else
		return;

	uint8_t &enabled = ::enabled[port];
	if (enable) {
		enabled |= b;
		*ie |= b;
		bool on = (v & b) != 0;
		if (on)
			*ies |= b;
		else
			*ies &= ~b;
		pins[port][bit_index(b)]->set_state(on);
	} else {
		enabled &= ~b;
		*ie &= ~b;
	}
	*ifg &= ~b;
}
