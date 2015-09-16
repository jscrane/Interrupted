/*
 * Simple breathing LED using hardware PWM and a millisecond timer
 */
#include <Interrupted.h>

#define TIMEOUT	300
#define LED	0
#define	MILLIS	1
#define OFF	2

Timer msec(MILLIS, 10);
Watchdog watchdog(OFF, TIMEOUT);
Devices devices;

void setup() {
	pinMode(LED, OUTPUT);
	devices.add(watchdog);
	devices.add(msec);
	devices.begin();

	// hardware PWM on PB0
	TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(CS01);
	OCR0A = 0;

	msec.enable();
}

void loop() {
	static byte freq = 20, inc = 1;

	switch (devices.select()) {
	case MILLIS:
		freq += inc;
		if (freq >= 240 || freq <= 19)
			inc = -inc;
		OCR0A = freq;
		msec.enable();
		break;
	case OFF:
		pinMode(LED, INPUT);
		msec.disable();
		break;
	}
}
