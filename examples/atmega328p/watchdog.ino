#include <Interrupted.h>

/*
 * An LED on pin 19 (PB5, D13), and
 * A pushbutton between pin 4 (PD2, D2, INT0) and ground.
 * A pullup resistor between INT0 and Vcc.
 *
 * The LED comes on for 1 second after reset or when the
 * pushbutton is pressed.
 */
#define LED	13
#define TIMER	1
#define BUTTON	2

Watchdog timer(TIMER, 1);
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
