#include <Energia.h>
#include "device.h"
#include "clock.h"

static Clock *c;

// see https://e2e.ti.com/support/microcontrollers/msp430/f/166/t/118916
bool Clock::begin() {
	c = this;

	PJSEL0 |= BIT4 | BIT5;			// XT1
	CSCTL0_H = 0xa5;			// unlock register
	CSCTL4 &= ~XT1OFF;
	do {
		CSCTL5 &= ~XT1OFFG;		// clear XT1 fault flag
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1 & OFIFG);
	CSCTL0_H = 0x01;			// lock register

	return true;
}

void Clock::_enable(bool e) {
	RTCCTL01 |= RTCHOLD;
	if (e)
		RTCCTL01 |= RTCRDYIE;
	else
		RTCCTL01 &= ~RTCRDYIE;
	RTCCTL01 &= ~RTCHOLD;
}

unsigned Clock::_sleepmode() {
	return LPM4_bits;
}

void Clock::set(uint8_t h, uint8_t m, uint8_t s) {
	RTCSEC = s;
	RTCMIN = m;
	RTCHOUR = h;
}

#pragma vector=RTC_VECTOR
__interrupt void rtc_isr(void)
{
	switch (RTCIV) {
	case RTCIV_NONE:
		break;
	case RTCIV_RTCRDYIFG:
		while (!(RTCRDY));
		if (c)
			c->update(RTCHOUR, RTCMIN, RTCSEC);
		break;
	}
        __bic_SR_register_on_exit(LPM4_bits | GIE);
}
