#include <Interrupted.h>

/*
 * The LED comes on for 1 second after reset or when the
 * pushbutton is pressed.
 */
#define TIMER	1

Watchdog timer(TIMER, 1);
Port p1;
Pin led(RED_LED, p1); 
Pin push2(PUSH2, p1);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(led);
	devices.add(push2);
	devices.begin();

	pinMode(PUSH2, INPUT_PULLUP);
	pinMode(RED_LED, OUTPUT);
	digitalWrite(RED_LED, HIGH);
}

void loop(void)
{
	switch (devices.select()) {
	case PUSH2:
		if (!push2.is_on())
			digitalWrite(RED_LED, HIGH);
		break;
	case TIMER:
		digitalWrite(RED_LED, LOW);
		break;
	case RED_LED:
		timer.enable(led.is_on());
		break;
	}
}
