#include <Interrupted.h>

/*
 * An LED on pin 5 (PB0, D0).
 *
 * The LED comes on for 1 second before being turned off by the timer.
 */
const int LED = 0;
Timer1 timer(1, 1000);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.begin();
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
}

void loop(void)
{
	timer.enable();
	devices.select();
	digitalWrite(LED, !digitalRead(LED));
}
