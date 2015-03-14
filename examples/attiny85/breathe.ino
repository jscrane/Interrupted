#include <Interrupted.h>

// time before sleep in millis
#define IDLE_MS 5 * 60000L
#define DEBOUNCE_MS 1000L

#define LED	0
#define TIMER	1
#define BUTTON	2

Devices devices;
External button(BUTTON, LOW);
Timer1 timer(TIMER, IDLE_MS);
Busy busy;

TinyDebugSerial serial;

void setup() {
	devices.add(button);
	devices.add(timer);
	devices.add(busy);
	devices.begin();

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	timer.enable();
	busy.enable();
}  

void loop() {
	static uint32_t last;
	static bool sleeping = false;
	static int spd = 55, freq = 20, inc = 1;
	uint32_t now = millis();

	switch (devices.select()) {
	case BUTTON:
		last = now;
		if (sleeping) {
			sleeping = false;
			pinMode(LED, OUTPUT);
			timer.enable();
			busy.enable();
		} else if (now - last > DEBOUNCE_MS) {
			spd += 10;
			if (spd > 95)
				spd = 15;
		}
		break;
	case TIMER:
		digitalWrite(LED, LOW);
		pinMode(LED, INPUT);
		busy.disable();
		sleeping = true;
		break;
	default:
		delayMicroseconds(spd * freq);
		digitalWrite(LED, LOW);
		delayMicroseconds(spd * (255 - freq));
		digitalWrite(LED, HIGH);
		freq += inc;
		if (freq >= 240 || freq <= 19)
			inc = -inc;
		break;
	}
}
