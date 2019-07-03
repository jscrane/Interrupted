#include <Energia.h>

#include "device.h"
#include "watchdog.h"
#include "atomic.h"

/*
 * Not _really_ a watchdog, because Energia uses the WDT to manage
 * millis(), micros(), etc. However this _does_ use the ACLK, which
 * remains active in LPM3, and runs off the VLOCLK at 12kHz.
 */
static Device *d;

#pragma vector=TIMER0_A0_VECTOR 
__interrupt void timer_a0(void)
{
        if (d)
                d->ready();
        __bic_SR_register_on_exit(LPM3_bits | GIE);
}

bool Watchdog::begin() {
	d = this;
	return false;
}

void Watchdog::_enable(bool e) {
	if (e)
		_left = _update_prescaler(_secs);
	else
		TA0CCTL0 &= ~CCIE;
}

unsigned Watchdog::_update_prescaler(unsigned t) {
	unsigned w = TASSEL__ACLK | MC__UP | TACLR;
	if (t >= 8) {
		w |= ID_3;	// divide by 8
		t -= 8;
	} else if (t >= 4) {
		w |= ID_2;	// divide by 4
		t -= 4;
	} else if (t >= 2) {
		w |= ID_1;	// divide by 2
		t -= 2;
	} else
		t = 0;

	Atomic block;
	TA0CCTL0 = CCIE;
	TA0CCR0 = 12000;
	TA0CTL |= w;
	return t;
}

unsigned Watchdog::_sleepmode() {
	return LPM3_bits;
}
