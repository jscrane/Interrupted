#include "device.h"
#include "external.h"
#include "pinchange.h"
#include "timer.h"

Watchdog timer(5, 1);
External int0(2), int1(3, RISING);
PinChangeGroup pins(D8_13);
PinChange led(pins, 13);
Devices devices;

void setup(void)
{
	Serial.begin(115200);

	devices.add(timer);
	devices.add(int0);
	devices.add(int1);
	devices.add(led);
	devices.begin();

	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
}

void loop(void)
{
	switch (devices.select()) {
	case 2:
		digitalWrite(13, HIGH);		// external interrupt #0
		break;
	case 1:
	case 3:
		digitalWrite(13, LOW);		// timer or ext interrupt #1
		break;
	case 13:
		int0.enable(!led.is_on());
		int1.enable(led.is_on());
		timer.enable(led.is_on());
		break;
	default:
		Serial.println("???");
		break;
	}

	Serial.println("awake");
	delay(100);
}
