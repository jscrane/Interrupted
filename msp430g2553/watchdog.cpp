#include <Energia.h>

#include "device.h"
#include "watchdog.h"

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
	if (!e) 
		TACCTL0 &= ~CCIE;
	else if (!(TACCTL0 & CCIE)) {
		TACCTL0 |= CCIE;
		TACCR0 = 12000;
		TACTL |= TASSEL_1 | MC_1 | TACLR;
		switch (_scale) {
		case 2:
			TACTL |= ID_1;
			break;
		case 4:
			TACTL |= ID_2;
			break;
		case 8:
			TACTL |= ID_3;
			break;
		}
	}
}

unsigned Watchdog::_sleepmode() {
	return LPM3_bits;
}
