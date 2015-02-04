#include "device.h"
#include "external.h"
#include "pinchange.h"
#include "timer.h"
#include "timer1.h"
#include "watchdog.h"

Timer1 timer(1, 2000);
External int0(2, LOW);
PinChangeGroup pins(PB);
PinChange led(0, pins);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(int0);
	devices.add(led);
	devices.begin();

	timer.enable(false);
	pinMode(0, OUTPUT);
	digitalWrite(0, HIGH);
}

void loop(void)
{
	switch (devices.select()) {
	case 2:				// external interrupt #0
		digitalWrite(0, HIGH);
		break;
	case 1:				// timer
		digitalWrite(0, LOW);
		break;
	case 0:
		timer.enable(led.is_on());
		break;
	}
}
