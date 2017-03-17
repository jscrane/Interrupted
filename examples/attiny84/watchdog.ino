#include <avr/wdt.h>
#include <Interrupted.h>

/*
 * An LED on pin 2 (PB0, D10), which turns on and off with the watchdog.
 */
#define LED	10
#define TIMER	1

Port portb;
Pin led(LED, portb); 
Watchdog timer(TIMER, 1);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(led);
	devices.begin();

	pinMode(LED, OUTPUT);
	timer.enable();
}

void loop(void)
{
	switch (devices.select()) {
	case LED:
		timer.enable();
		break;
	case TIMER:
		digitalWrite(LED, !digitalRead(LED));
		break;
	}
}
