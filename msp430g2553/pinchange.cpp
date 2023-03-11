#include <Arduino.h>

#include "device.h"
#include "pinchange.h"

static Port *d[2];

#pragma vector=PORT1_VECTOR 
__interrupt void port1(void)
{
	if (d[0])
		d[0]->ready();
	__bic_SR_register_on_exit(LPM4_bits | GIE);
}

#pragma vector=PORT2_VECTOR
__interrupt void port2(void)
{
	if (d[1])
		d[1]->ready();
	__bic_SR_register_on_exit(LPM4_bits | GIE);
}

void Port::ready() {
	volatile byte *ifg, *ies;
	byte v = 0;
	if (_port == P1) {
		ifg = &P1IFG;
		ies = &P1IES;
		v = P1IN;
	} else if (_port == P2) {
		ifg = &P2IFG;
		ies = &P2IES;
		v = P2IN;
	} else
		return;
	for (int i = 0; i < 8; i++) {
		byte b = bit(i);
		// deal with the interrupt here whether the device
		// is enabled or not: ensure the next edge trigger
		// remains consistent with its current state
		// (i.e., don't just toggle it as in the examples).
		if (*ifg & b) {
			if (_enabled & b)
				_pins[i]->set_state((v & b) != 0);
			if (_pins[i]->is_high())
				*ies |= b;	// next trigger on hi->lo
			else
				*ies &= ~b;	// next trigger on lo->hi
			*ifg &= ~b;
		}
	}
}

void Port::add_pin(int pin, Pin *p) {
	byte b = digitalPinToBitMask(pin);
	_pins[bit_index(b)] = p;
	if (!_port)
		_port = digitalPinToPort(pin);
	if (_port == P1)
		d[0] = this;
	else if (_port == P2)
		d[1] = this;
}

void Port::enable_pin(int pin, bool enable) {
	byte b = digitalPinToBitMask(pin);
	byte v = 0;
	volatile byte *ie, *ifg, *ies;
	if (_port == P1) {
		ie = &P1IE;
		ifg = &P1IFG;
		ies = &P1IES;
		v = P1IN;
	} else if (_port == P2) {
		ie = &P2IE;
		ifg = &P2IFG;
		ies = &P2IES;
		v = P2IN;
	} else
		return;
	if (enable) {
		_enabled |= b;
		*ie |= b;
		bool on = (v & b) != 0;
		if (on)
			*ies |= b;
		else
			*ies &= ~b;
		_pins[bit_index(b)]->set_state(on);
	} else {
		_enabled &= ~b;
		*ie &= ~b;
	}
	*ifg &= ~b;
}
