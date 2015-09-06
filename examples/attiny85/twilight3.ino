#include <Interrupted.h>

#define LDR	A1
#define PIR	1
#define LED	0
#define TIMER	2

Analog ldr(LDR);
Port portb;
Pin pir(PIR, portb);
Pin led(LED, portb);
Watchdog timer(TIMER, 1);
Devices devices;

#define SAMPLES		30
#define DARK		100
#define LIGHT		150
#define ONTIME		60

int smoothed = 0, ontime = 0;

unsigned sample(int curr) {
	static int samples[SAMPLES], pos = 0;
	static long total = 0, ns = 0;

	if (ns < SAMPLES) ns++;
	total += curr - samples[pos];
	samples[pos++] = curr;
	if (pos == SAMPLES) pos = 0;
	return (unsigned)(total / ns);
}

#define SLOW 250
#define FAST 125

void blink(int d) {
	int n = 500 / d;
  	for (int i = 0; i < n; i++) {
		digitalWrite(LED, HIGH);
		delay(d);
		digitalWrite(LED, LOW);
		delay(d);
	}
}

void setup() {
	devices.add(pir);
	devices.add(ldr);
	devices.add(timer);
	devices.add(led);
	devices.begin();

	pinMode(PIR, INPUT);
	pinMode(LED, OUTPUT);
	blink(FAST);
	timer.enable();
}

void loop() {
	switch (devices.select()) {
	case TIMER:
		ldr.enable();
		break;
	case LDR:
		smoothed = sample(ldr.read());
		timer.enable();
		if (pir.is_on() && smoothed < DARK) {
			digitalWrite(LED, HIGH);
			ontime = ONTIME;
		} else if ((!pir.is_on() || smoothed > LIGHT) && ontime > 0)
			ontime--;
		else if (ontime == 0) {
			blink(SLOW);
			digitalWrite(LED, LOW);
		}
		break;
	}
}
