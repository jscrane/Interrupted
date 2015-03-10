#include <Interrupted.h>

#define LED	0
#define TIMER	1
#define BUTTON	2

Watchdog timer(TIMER, 1);
External button(BUTTON, LOW);
PinChangeGroup pins(PB);
PinChange led(LED, pins); 
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(led);
	devices.add(button);
	devices.begin();

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
}

void loop(void)
{
	switch (devices.select()) {
	case BUTTON:
		digitalWrite(LED, HIGH);
		break;
	case TIMER:
		digitalWrite(LED, LOW);
		break;
	case LED:
		timer.enable(led.is_on());
		break;
	}
}
