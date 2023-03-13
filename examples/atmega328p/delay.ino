#include <Interrupted.h>

/*
 * An LED on pin 19 (PB5, D13).
 *
 * The LED flashes with a duty-cycle of 500ms.
 */
#define TIMER	1
Delay timer(TIMER, 500);
Devices devices(timer);

void setup(void)
{
	devices.powersave();
	devices.begin();

	pinMode(LED_BUILTIN, OUTPUT);
	timer.enable();
}

void loop(void)
{
	static bool on;
	if (devices.select() == TIMER) {
		on = !on;
		digitalWrite(LED_BUILTIN, on? HIGH: LOW);
		timer.enable();
	}
}
