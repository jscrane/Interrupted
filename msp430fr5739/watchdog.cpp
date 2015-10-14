#include <Energia.h>

#include "device.h"
#include "atimer.h"
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
		TA0CCTL0 &= ~CCIE;
	else if (!(TA0CCTL0 & CCIE)) {
		// Timer0_A3
		TA0CCR0 = 12000;
		TA0CTL |= TASSEL__ACLK | MC__UP | TACLR;
		switch (_scale) {
		case 2:
			TA0CTL |= ID_1;	// divide by 2
			break;
		case 4:
			TA0CTL |= ID_2;	// divide by 4
			break;
		case 8:
			TA0CTL |= ID_3;	// divide by 8
			break;
		}
		// enable interrupts
		TA0CCTL0 = CCIE;
	}
}

void Watchdog::_prescale() {
	disable();
}

unsigned Watchdog::_sleepmode() {
	return LPM3_bits;
}
