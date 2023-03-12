#include <Interrupted.h>

/*
 * PUSH1 for time-adjust mode, PUSH2 to advance hh/mm, PUSH1 to restart.
 * The RTC requires a 32.768kHz crystal.
 */
class Clock: public Device {
public:
	Clock(int id): Device(id) {}

	void set(uint8_t h, uint8_t m, uint8_t s) {
		RTCSEC = s;
		RTCMIN = m;
		RTCHOUR = h;
	}

	// enabled by default
	bool begin();

	uint8_t hour() { return _h; }
	uint8_t mins() { return _m; }
	uint8_t secs() { return _s; }

	void update(uint8_t h, uint8_t m, uint8_t s) {
		_h = h; _m = m; _s = s;
		ready();
	}

protected:
	void _enable(bool);
	unsigned _sleepmode() {
		return LPM4_bits;
	}

private:
	volatile uint8_t _h, _m, _s;
};

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

#define RTC	1
#define SEROUT	99

SerialOut<> output(SEROUT, TERMINAL_SPEED);
Clock clock(RTC);
Port p4;
Pin push1(PUSH1, p4);
Pin push2(PUSH2, p4);
Devices devices(output, clock, push1, push2);

void setup(void)
{
	pinMode(PUSH1, INPUT_PULLUP);
	pinMode(PUSH2, INPUT_PULLUP);
	clock.set(12, 30, 00);

	devices.begin();
}

void loop(void)
{
	static bool colon = false, hour_mode = false;
	static char buf[16];
	uint8_t h = clock.hour(), m = clock.mins(), s = clock.secs();

	switch (devices.select()) {
	case RTC:
		colon = !colon;
		break;
	case PUSH1:
		if (push1.is_on())
			return;
		if (clock.is_enabled()) {
			clock.disable();
			push2.enable();
			hour_mode = true;
		} else if (hour_mode)
			hour_mode = false;
		else {
			push2.disable();
			clock.set(h, m, 0);
			clock.enable();
		}
		break;
	case PUSH2:
		if (push2.is_on())
			return;
		if (hour_mode)
			h++;
		else
			m++;
		clock.update(h % 24, m % 60, 0);
		break;
	case SEROUT:
	default:
		return;
	}
	char c = colon? ':': ' ';
	sprintf(buf, "%02d%c%02d%c%02d\r", h, c, m, c, s);
	output.print(buf);
}
