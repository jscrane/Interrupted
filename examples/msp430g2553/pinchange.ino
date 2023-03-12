#include <Interrupted.h>

Port p1;
Pin led(RED_LED, p1);
Pin push2(PUSH2, p1);
Devices devices(led, push2);

void setup()
{
	pinMode(RED_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
	pinMode(PUSH2, INPUT_PULLUP);
	digitalWrite(RED_LED, LOW);
	digitalWrite(GREEN_LED, HIGH);

	devices.begin();
}

void loop()
{
	switch (devices.select()) {
	case RED_LED:
		digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
		break;
	case PUSH2:
		if (!push2.is_on())
			digitalWrite(RED_LED, !digitalRead(RED_LED));	
		break;
	}
}
