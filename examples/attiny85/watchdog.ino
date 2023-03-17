#include <Interrupted.h>

/*
 * An LED on pin 5 (PB0, D0).
 *
 * The LED comes on for 1 second before being turned off by the timer.
 */
const int LED = 0;
const int TIMER = 1;
Watchdog timer(TIMER);
Devices devices(timer);

void setup(void)
{
	devices.powersave();
	devices.begin();
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
}

void loop(void)
{
	timer.enable();
	if (devices.select() == TIMER)
		digitalWrite(LED, !digitalRead(LED));
}
