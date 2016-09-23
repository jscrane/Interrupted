#include <Interrupted.h>

/*
 * An LED on pin 19 (PB5, D13).
 *
 * The LED flashes with a duty-cycle of 500ms.
 */
#define LED	13
#define TIMER	1
Timer timer(TIMER, 500);
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
	if (timer.is_ready())
	{
		digitalWrite(LED, !digitalRead(LED));
	}
}
