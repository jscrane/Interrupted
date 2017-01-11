#include <Interrupted.h>

/*
 * An LED on pin 19 (PB5, D13),
 * A pushbutton between pin 4 (PD2, D2, INT0) and ground.
 *
 * The LED comes on for 1 second after reset or when the
 * pushbutton is pressed.
 */
#define LED	13
#define TIMER	1
#define BUTTON	2

Watchdog timer(TIMER);
External button(BUTTON, LOW);
Port portb;
Pin led(LED, portb); 
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(led);
	devices.add(button);
	devices.begin();

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	digitalWrite(BUTTON, INPUT_PULLUP);
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
