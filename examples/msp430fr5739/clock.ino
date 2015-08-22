#include <Interrupted.h>
#include <clock.h>

Clock clock(1);
Devices devices;

void setup(void)
{
	Serial.begin(9600);
	clock.set(0, 0, 0);
	devices.add(clock);
	devices.begin();
}

void loop(void)
{
	devices.select();
	uint8_t h = clock.hour(), m = clock.mins(), s = clock.secs();
	if (h < 10) Serial.print("0");
	Serial.print(h);
	Serial.print(":");
	if (m < 10) Serial.print("0");
	Serial.print(m);
	Serial.print(":");
	if (s < 10) Serial.print("0");
	Serial.println(s);
}
