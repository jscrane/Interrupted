#include <Interrupted.h>

/*
 * An LED on pin 5 (PB0, D0), and
 * A pushbutton between pin 7 (PB2, D2, INT0) and ground.
 *
 * While the Tiny is awake, it implements a software PWM on the LED,
 * which appears to change in intensity at a constant rate. This rate
 * is increased by pressing the button. After a minute, the Timer
 * fires and the Tiny sleeps until the next button press.
 */

// time before sleep in millis
#define IDLE_MS 60000L
#define DEBOUNCE_MS 1000L

#define LED	0
#define TIMER	1
#define BUTTON	2

External button(BUTTON, LOW);
Delay timer(TIMER, IDLE_MS);
Busy busy;
Devices devices(button, timer, busy);

void setup() {
	devices.begin();

	pinMode(BUTTON, INPUT_PULLUP);
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
