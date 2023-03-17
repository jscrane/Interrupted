#include <Interrupted.h>

/*
 * A built-in LED on pin 5 (PB2, D8).
 * The duty cycle of the LED increases exponentially.
 */
unsigned dt = 100;
Timer timer(1, dt);
Devices devices(timer);

void setup(void)
{
	devices.begin();
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop(void)
{
	timer.enable();
	if (devices.select() == 1) {
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		dt *= 2;
		timer.delay(dt);
	}
}
