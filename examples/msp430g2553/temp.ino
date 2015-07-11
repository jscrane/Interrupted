#include <Interrupted.h>

/*
 * Reads the internal temperature sensor (A10 on Energia).
 * http://forum.43oh.com/topic/1954-using-the-internal-temperature-sensor/
 */
Analog adc(TEMPSENSOR, INTERNAL2V5);
Devices devices;

void setup(void)
{
	devices.add(adc);
	devices.begin();

	Serial.begin(9600);
}

void loop(void)
{
	adc.enable();
	devices.select();
	unsigned a = adc.read();
	int c = ((45115L * a) - 18169625L) >> 16;
	Serial.println(c);
}
