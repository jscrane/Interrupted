#include <Interrupted.h>

/*
 * Built-in LED_BUILTIN on hardware pin 19 (PB5, D13),
 * A pushbutton between pin 4 (PD2, D2, INT0) and ground.
 *
 * The LED_BUILTIN comes on for 1 second after reset or when the
 * pushbutton is pressed.
 */
#define TIMER	1
#define BUTTON	2

Watchdog timer(TIMER);
External button(BUTTON, LOW);
Pin led(LED_BUILTIN);
Devices devices(timer, led, button);

void setup(void)
{
	devices.begin();
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
}

void loop(void)
{
	switch (devices.select()) {
	case BUTTON:
		digitalWrite(LED_BUILTIN, HIGH);
		break;
	case TIMER:
		digitalWrite(LED_BUILTIN, LOW);
		break;
	case LED_BUILTIN:
		timer.enable(led.is_on());
		break;
	}
}
