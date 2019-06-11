#include <Interrupted.h>

/*
 * Reads the internal temperature sensor (A10 on Energia).
 * http://forum.43oh.com/topic/1954-using-the-internal-temperature-sensor/
 */
#define SEROUT  99

Analog adc(TEMPSENSOR, INTERNAL2V5);
SerialOut<> output(SEROUT, TERMINAL_SPEED);
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
	switch (devices.select()) {
	case TEMPSENSOR:
		output.println(((45115L * adc.read()) - 18169625L) >> 16);
		break;
	case SEROUT:
		adc.enable();
		break;
	}
}
