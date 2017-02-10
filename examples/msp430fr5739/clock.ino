#include <Interrupted.h>
#include <clock.h>

/*
 * PUSH1 for time-adjust mode, PUSH2 to advance hh/mm, PUSH1 to restart.
 * The RTC requires a 32.768kHz crystal.
 */
#define RTC	1
#define SEROUT	99

SerialOut output(SEROUT, 9600);
Clock clock(RTC);
Port p4;
Pin push1(PUSH1, p4);
Pin push2(PUSH2, p4);
Devices devices;

void setup(void)
{
	pinMode(PUSH1, INPUT_PULLUP);
	pinMode(PUSH2, INPUT_PULLUP);
	clock.set(12, 30, 00);

	devices.add(push1);
	devices.add(push2);
	devices.add(clock);
	devices.add(output);
	devices.begin();
}

void loop(void)
{
	static bool colon = false, hour_mode = false;
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
			clock.set(h, m, s);
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
	char buf[16], c = colon? ':': ' ';
	sprintf(buf, "%02d%c%02d%c%02d\r", h, c, m, c, s);
	output.write(buf);
}
