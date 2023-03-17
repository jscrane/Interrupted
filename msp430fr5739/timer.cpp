#include <Arduino.h>

#include "atomic.h"
#include "device.h"
#include "timer.h"

static Device *d;

#pragma vector=TIMER0_B0_VECTOR 
__interrupt void timer_b0(void)
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
		TB0CCR0 = F_CPU / 1000;
		TB0CTL |= TBSSEL__SMCLK | MC__UP | TBCLR;
		TB0CCTL0 |= CCIE;
	} else
		TB0CCTL0 &= ~CCIE;
}

unsigned Timer::_sleepmode() {
	return LPM1_bits;
}
