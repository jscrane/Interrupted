#include <Interrupted.h>

/*
 * The LED comes on for 1.5 seconds after reset or when the
 * pushbutton is pressed.
 */
#define TIMER	1

Timer timer(TIMER, 1500);
Port p1;
Pin led(RED_LED, p1); 
Pin push2(PUSH2, p1);
Devices devices(timer, led, push2);

void setup(void)
{
	pinMode(PUSH2, INPUT_PULLUP);
	pinMode(RED_LED, OUTPUT);
	digitalWrite(RED_LED, HIGH);
	devices.begin();
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
