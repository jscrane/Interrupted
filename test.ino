#include "device.h"
#include "external.h"

External ext0(2), ext1(3);
Devices devices;

void setup(void)
{
	Serial.begin(115200);
	Serial.println("hello");

	devices.add(ext0);
	devices.add(ext1);
	devices.begin();

	pinMode(13, OUTPUT);
}

void loop(void)
{
	switch (devices.select()) {
	case 2:
		digitalWrite(13, HIGH);
		break;
	case 3:
		digitalWrite(13, LOW);
		break;
	default:
		Serial.println("???");
		break;
	}

	Serial.println("awake");
	delay(100);
}
