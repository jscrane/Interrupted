#include <Interrupted.h>

/*
 * The LED comes on for 1.5 seconds after reset or when the
 * pushbutton is pressed.
 */
#define TIMER	1

Timer timer(TIMER, 1500);
PinChangeGroup p3, p4;
PinChange led(LED5, p3); 
PinChange push2(PUSH2, p4);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(led);
	devices.add(push2);
	devices.begin();

	pinMode(LED5, OUTPUT);
	pinMode(PUSH2, INPUT_PULLUP);
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
		break;
	case LED5:
		timer.enable(led.is_on());
		break;
	}
}
