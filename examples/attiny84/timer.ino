#include <Interrupted.h>

/*
 * An LED on pin 10 (PA3, D3).
 */
const int LED = 3;
unsigned dt = 100;
Timer1 timer(1, dt);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.begin();
	pinMode(LED, OUTPUT);
}

void loop(void)
{
	timer.enable();
	devices.select();
	digitalWrite(LED, !digitalRead(LED));
	dt *= 2;
	timer.delay(dt);
}
