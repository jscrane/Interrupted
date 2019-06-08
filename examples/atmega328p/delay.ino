#include <Interrupted.h>

/*
 * An LED on pin 19 (PB5, D13).
 *
 * The LED flashes with a duty-cycle of 500ms.
 */
#define TIMER	1
Delay timer(TIMER, 500);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.begin();

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	timer.enable();
}

void loop(void)
{
	if (devices.select() == TIMER) {
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		timer.enable();
	}
}
