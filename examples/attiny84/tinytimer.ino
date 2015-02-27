#include "device.h"
#include "timer.h"
#include "timer1.h"

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
	timer.enable(true);
	devices.select();
	digitalWrite(LED, !digitalRead(LED));
	dt *= 2;
	timer.delay(dt);
}
