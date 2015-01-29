#include "device.h"
#include "external.h"
#include "pinchange.h"
#include "timer.h"

Watchdog timer(5, 1);
External int0(2), int1(3, RISING);
PinChangeGroup group(D8_13);
PinChangePin led(group, 13);
Devices devices;

void setup(void)
{
	Serial.begin(115200);

	devices.add(timer);
	devices.add(int0);
	devices.add(int1);
	devices.add(led);
	devices.begin();

	int0.enable(false);		// LED initially on
	timer.enable(false);		// timer off

	pinMode(13, OUTPUT);
}

void loop(void)
{
	switch (devices.select()) {
	case 1:	// fall through...
	case 2:
		digitalWrite(13, HIGH);
		int0.enable(false);
		timer.enable(false);
		int1.enable(true);
		break;
	case 3:
		digitalWrite(13, LOW);
		int0.enable(true);
		timer.enable(true);
		int1.enable(false);
		break;
	case 13:
		Serial.println("LED!");
		break;
	default:
		Serial.println("???");
		break;
	}

	Serial.println("awake");
	delay(100);
}
