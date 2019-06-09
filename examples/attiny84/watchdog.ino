#include <avr/wdt.h>
#include <Interrupted.h>

/*
 * A built-in LED on pin 5 (PB2, D8).
 */
#define TIMER	1

Port portb;
Pin led(LED_BUILTIN, portb); 
Watchdog timer(TIMER, 1000);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(led);
	devices.begin();

	pinMode(LED_BUILTIN, OUTPUT);
	timer.enable();
}

void loop(void)
{
	switch (devices.select()) {
	case LED_BUILTIN:
		timer.enable();
		break;
	case TIMER:
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		break;
	}
}
