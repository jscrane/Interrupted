#include <Interrupted.h>

/*
 * Built-in LED on hardware pin 19 (PB5, D13).
 *
 * The LED flashes with a duty-cycle of 500ms.
 */
#define TIMER	1
Timer timer(TIMER, 500);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.begin();

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
}

void loop(void)
{
	timer.enable();
	if (devices.select() == TIMER)
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
