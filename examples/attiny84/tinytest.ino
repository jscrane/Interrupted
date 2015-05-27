#include <avr/wdt.h>
#include <Interrupted.h>

#define LED	3
#define TIMER	1
#define EXT0	2

Watchdog timer(TIMER, 1);
External int0(EXT0, LOW);
PinChangeGroup porta;
PinChange led(LED, porta); 
Devices devices;
Analog adc(A3, vcc);

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
	unsigned val = 0;

	switch (devices.select()) {
	case EXT0:
		digitalWrite(LED, HIGH);
		break;
	case LED:
		adc.enable(led.is_on());
		break;
	case A3:
		val = adc.read();
		timer.delay(val / 50);
		timer.enable(led.is_on());
		break;
	case TIMER:
		digitalWrite(LED, LOW);
		break;
	}
}
