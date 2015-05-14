#include <Energia.h>

#include "device.h"
#include "pinchange.h"

static PinChangeGroup *d[2];

#pragma vector=PORT1_VECTOR 
__interrupt void port1(void)
{
	if (d[0])
		d[0]->ready();
//	digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
	__bic_SR_register_on_exit(LPM0_bits | GIE);
}

#pragma vector=PORT2_VECTOR
__interrupt void port2(void)
{
	if (d[1])
		d[1]->ready();
	//__bic_SR_register_on_exit(LPM3_bits);
}

void PinChangeGroup::ready() {
	volatile byte *ifg, *ies;
	byte v = 0;
	if (_port == P1) {
		ifg = &P1IFG;
		ies = &P1IES;
		v = P1IN;
	} else {
		ifg = &P2IFG;
		ies = &P2IES;
		v = P2IN;
	}
	for (int i = 0; i < 8; i++) {
		byte b = bit(i);
		if (_pins[i] && (_enabled & b) && (*ifg & b)) {
			_pins[i]->set_state((v & b) != 0);
//			*ies ^= b;	// toggle interrupt edge
		}
	}
	*ifg = 0;
}

void PinChangeGroup::add_pin(int pin, PinChange *p) {
	byte b = digitalPinToBitMask(pin);
	int i = 0;
	while (b >>= 1)
		i++;
	_pins[i] = p;
	if (_port == P1)
		d[0] = this;
	else if (_port == P2)
		d[1] = this;
}

void PinChangeGroup::enable_pin(int pin, bool enable) {
	byte b = digitalPinToBitMask(pin);
	volatile byte *ie, *ifg;
	if (_port == P1) {
		ie = &P1IE;
		ifg = &P1IFG;
	} else {
		ie = &P2IE;
		ifg = &P2IFG;
	}
	if (enable) {
		_enabled |= b;
		*ie |= b;
		*ifg &= ~b;
	} else {
		_enabled &= ~b;
		*ie &= ~b;
	}
}
