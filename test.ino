#include "device.h"
#include "external.h"
#include "timer.h"

Watchdog timer(5, 1);
External ext0(2), ext1(3, RISING);
Devices devices;

void setup(void)
{
	Serial.begin(115200);

	devices.add(timer);
	devices.add(ext0);
	devices.add(ext1);
	devices.begin();
	ext0.enable(false);		// LED initially on
	timer.enable(false);		// timer off

	pinMode(13, OUTPUT);
}

void loop(void)
{
	switch (devices.select()) {
	case 1:	// fall through...
	case 2:
		digitalWrite(13, HIGH);
		ext0.enable(false);
		timer.enable(false);
		ext1.enable(true);
		break;
	case 3:
		digitalWrite(13, LOW);
		ext0.enable(true);
		timer.enable(true);
		ext1.enable(false);
		break;
	default:
		Serial.println("???");
		break;
	}

	Serial.println("awake");
	delay(100);
}
