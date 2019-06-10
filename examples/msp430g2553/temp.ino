#include <Interrupted.h>

/*
 * Reads the internal temperature sensor (A10 on Energia).
 * http://forum.43oh.com/topic/1954-using-the-internal-temperature-sensor/
 */
#define SEROUT  99

Analog adc(TEMPSENSOR, INTERNAL2V5);
SerialOut output(SEROUT, TERMINAL_SPEED);
Devices devices;

void setup(void)
{
	devices.add(adc);
	devices.add(output);
	devices.begin();
	adc.enable();
}

void loop(void)
{
	int c;
	char buf[8];

	switch (devices.select()) {
	case TEMPSENSOR:
		c = ((45115L * adc.read()) - 18169625L) >> 16;
		itoa(c, buf, 10);
		strcat(buf, "\r\n");
		output.write(buf);
		break;
	case SEROUT:
		adc.enable();
		break;
	}
}
