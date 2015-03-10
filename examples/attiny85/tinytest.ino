#include <Interrupted.h>

#define LED	0
#define TIMER	1
#define BUTTON	2

Watchdog timer(TIMER, 1);
External button(BUTTON, LOW);
PinChangeGroup pins(PB);
PinChange led(LED, pins); 
//PinChange button(BUTTON, pins); 
Devices devices;
//Analog adc(A3, vcc);

void setup(void)
{
	devices.add(timer);
//	devices.add(int0);
	devices.add(led);
	devices.add(button);
//	devices.add(adc);
	devices.begin();

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
}

void loop(void)
{
//	unsigned val = 0;

	switch (devices.select()) {
	case BUTTON:
		digitalWrite(LED, HIGH);
		break;
	case LED:
//		adc.enable(led.is_on());
		if (led.is_on())
			timer.enable();
		break;
//	case A3:
//		val = adc.read();
//		timer.delay(val / 50);
//		timer.enable(led.is_on());
//		break;
	case TIMER:
		digitalWrite(LED, LOW);
		break;
	}
}
