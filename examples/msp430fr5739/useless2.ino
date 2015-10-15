#include <Interrupted.h>

/*
 * The LED comes on for 2.5 seconds after reset or when the
 * pushbutton is pressed.
 *
 * This version uses the Delay timer.
 */
#define TIMER	1

Delay timer(TIMER, 2500);
Port p3, p4;
Pin led(LED5, p3); 
Pin push2(PUSH2, p4);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(led);
	devices.add(push2);
	devices.begin();

	pinMode(PUSH2, INPUT_PULLUP);
	pinMode(LED5, OUTPUT);
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
