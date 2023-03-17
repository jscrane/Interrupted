#include <Interrupted.h>

/*
 * The LED comes on for 2.5 seconds after reset or when the
 * pushbutton is pressed.
 *
 * This version uses the Delay timer.
 */
#define TIMER	1

Delay timer(TIMER, 2500);
Pin led(LED5);
Pin push2(PUSH2);
Devices devices(timer, led, push2);

void setup(void)
{
	devices.begin();

	pinMode(PUSH2, INPUT_PULLUP);
	pinMode(LED5, OUTPUT);
	digitalWrite(LED5, HIGH);
}

void loop(void)
{
	switch (devices.select()) {
	case PUSH2:
		if (!push2.is_on())
			digitalWrite(LED5, HIGH);
		break;
	case TIMER:
		digitalWrite(LED5, LOW);
		return;
	case LED5:
		timer.enable(led.is_on());
		break;
	}
}
