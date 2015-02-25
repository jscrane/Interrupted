#include <avr/wdt.h>
#include "device.h"
#include "external.h"
#include "pinchange.h"
#include "timer.h"
#include "timer1.h"
#include "watchdog.h"
#include "adc.h"

#define LED	10
#define TIMER	1
#define EXT0	2

Watchdog timer(TIMER, 1);
External int0(EXT0, LOW);
PinChangeGroup pins(PA);
PinChange led(LED, pins); 
Devices devices;
Analog adc(A3);

void setup(void)
{
	devices.add(timer);
	devices.add(int0);
	devices.add(led);
	devices.add(adc);
	devices.begin();

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
}

void loop(void)
{
	switch (devices.select()) {
	case EXT0:
		digitalWrite(LED, HIGH);
		break;
	case TIMER:
		digitalWrite(LED, LOW);
		break;
	case LED:
		timer.enable(led.is_on());
		adc.enable(led.is_on());
		break;
	case A3:
		adc.enable(led.is_on());
		break;
	}
}
