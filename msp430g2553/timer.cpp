#include <Energia.h>

#include "atomic.h"
#include "device.h"
#include "timer.h"

static Device *d;

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1_a0(void)
{
        if (d)
                d->ready();
        __bic_SR_register_on_exit(LPM1_bits | GIE);
}

bool Timer::begin() {
	d = this;
	return false;
}

void Timer::_enable(bool e) {
	if (e) {
		TA1CCTL0 |= CCIE;
		TA1CCR0 = F_CPU / 1000;
		TA1CTL |= TASSEL_2 | MC_1 | TACLR;
	} else
		TA1CCTL0 &= ~CCIE;
}

unsigned Timer::_sleepmode() {
	return LPM1_bits;
}
