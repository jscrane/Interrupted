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

	pinMode(13, OUTPUT);
}

void loop(void)
{
	switch (devices.select()) {
	case 1:
		Serial.println("tick");
		break;
	case 2:
		digitalWrite(13, HIGH);
		ext0.enable(false);	// only allow ext1 to interrupt now
		ext1.enable(true);
		break;
	case 3:
		digitalWrite(13, LOW);
		ext0.enable(true);	// only allow ext0 to interrupt now
		ext1.enable(false);
		break;
	default:
		Serial.println("???");
		break;
	}

	Serial.println("awake");
	delay(100);
}
