/*
 * Simple breathing LED using hardware PWM and a millisecond timer.
 * (see breathe.ino for setup)
 */
#include <Interrupted.h>
#include <avr/wdt.h>
#include <avr/power.h>

#define TIMEOUT	300
#define LED	0
#define	MILLIS	1
#define SECONDS	2

Timer msec(MILLIS, 10);
Watchdog watchdog(SECONDS);
Devices devices;

void setup() {
	pinMode(LED, OUTPUT);
	devices.add(watchdog);
	devices.add(msec);
	devices.begin(LOW_POWER);

	// hardware PWM on PB0
	power_timer0_enable();
	TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(CS01);
	OCR0A = 0;

	msec.enable();
	watchdog.enable();
}

void loop() {
	static byte freq = 20, inc = 1;
	static unsigned secs = 0;

	switch (devices.select()) {
	case MILLIS:
		freq += inc;
		if (freq >= 240 || freq <= 19)
			inc = -inc;
		OCR0A = freq;
		msec.enable();
		break;
	case SECONDS:
		if (++secs > TIMEOUT) {
			pinMode(LED, INPUT);
			msec.disable();
		} else
			watchdog.enable();
		break;
	}
}
