#include <Interrupted.h>

/*
 * An LED on pin 5 (PB0, D0), and
 * A pushbutton between pin 7 (PB2, D2, INT0) and ground.
 *
 * The LED comes on for 1 second after reset or when the
 * pushbutton is pressed.
 */
#define LED	0
#define TIMER	1
#define BUTTON	2

Watchdog timer(TIMER);
External button(BUTTON, LOW);
Port portb;
Pin led(LED, portb); 
Devices devices(timer, led, button);

void setup(void)
{
	devices.begin(LOW_POWER);

	pinMode(BUTTON, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);

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
