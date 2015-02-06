#include <avr/wdt.h>
#include "device.h"
#include "external.h"
#include "pinchange.h"
#include "timer.h"
#include "timer1.h"
#include "watchdog.h"

#define LED	10

Watchdog timer(1, 1);
External int0(2, LOW);
PinChangeGroup pins(PA);
PinChange led(LED, pins); 
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(int0);
	devices.add(led);
	devices.begin();

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
}

void loop(void)
{
	switch (devices.select()) {
	case 2:				// external interrupt #0
		digitalWrite(LED, HIGH);
		break;
	case 1:				// timer
		digitalWrite(LED, LOW);
		break;
	case LED:
		timer.enable(led.is_on());
		break;
	}
}
