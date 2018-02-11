#include <Energia.h>

#include "atomic.h"
#include "device.h"
#include "timer.h"

/*
 * Unfortunately uses the same timer infrastructure as the Watchdog.
 * However they shouldn't both be necessary simultaneously surely?
 */
static Device *d;

#pragma vector=TIMER0_A0_VECTOR 
__interrupt void timer_a0(void)
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
		TACCTL0 |= CCIE;
		TACCR0 = F_CPU / 1000;
		TACTL |= TASSEL_2 | MC_1 | TACLR;
	} else
		TACCTL0 &= ~CCIE;
}

unsigned Timer::_sleepmode() {
	return LPM1_bits;
}
