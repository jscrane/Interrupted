#include <Interrupted.h>

PinChangeGroup pins(P1);
PinChange led(RED_LED, pins);
PinChange push2(PUSH2, pins);
Devices devices;

void setup()
{
	devices.add(led);
	devices.add(push2);
	devices.begin();

	pinMode(RED_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
	pinMode(PUSH2, INPUT_PULLUP);
	digitalWrite(RED_LED, HIGH);
}

void loop()
{
	switch (devices.select()) {
	case RED_LED:
		// do nothing (yet)
		digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
		break;
	case PUSH2:
		digitalWrite(RED_LED, !digitalRead(RED_LED));	
		break;
	}
}
