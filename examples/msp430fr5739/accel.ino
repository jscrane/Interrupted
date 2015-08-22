#include <Interrupted.h>

/*
 * ACC_X on A6
 */
Analog adc(A6);
Devices devices;

#define MIN	400
#define MAX	620

const int leds[] = { 25, 25, 27, 28, 7, 8, 9, 10 };
int last = 0;

void setup(void)
{
	devices.add(adc);
	devices.begin();

	pinMode(ACC_ENABLE, OUTPUT);
	digitalWrite(ACC_ENABLE, HIGH);
	for (int i = 0; i < 8; i++) {
		pinMode(leds[i], OUTPUT);
		digitalWrite(leds[i], LOW);
	}
}

void loop(void)
{
	adc.wake();
	adc.enable();
	devices.select();
	int v = adc.read();
	int curr = (v - MIN)*8 / (MAX - MIN);
	if (v >= MIN && v <= MAX && last != curr) {
		digitalWrite(leds[last], LOW);
		digitalWrite(leds[curr], HIGH);
		last = curr;
	}
	adc.sleep();
}
