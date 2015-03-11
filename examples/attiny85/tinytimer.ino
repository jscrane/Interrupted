#include <Interrupted.h>

const int LED = 0;
unsigned dt = 1000;
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
	timer.delay(dt);
}
