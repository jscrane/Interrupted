#include <Interrupted.h>

/*
 * A built-in LED on pin 5 (PB2, D8).
 */
#define TIMER	1

Pin led(LED_BUILTIN);
Watchdog timer(TIMER, 3);
Devices devices(timer, led);

void setup(void)
{
	devices.begin();

	pinMode(LED_BUILTIN, OUTPUT);
	timer.enable();
}

void loop(void)
{
	switch (devices.select()) {
	case LED_BUILTIN:
		timer.enable();
		break;
	case TIMER:
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		break;
	}
}
