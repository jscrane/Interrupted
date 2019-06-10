#include <Interrupted.h>

/*
 * An analog input on pin 6, A4 (e.g., LDR/resistor voltage divider).
 */
#define SEROUT  99

Analog adc(A4, INTERNAL2V5);
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
	char buf[8];

	switch (devices.select()) {
	case A4:
		itoa(adc.read(), buf, 10);
		strcat(buf, "\r\n");
		output.write(buf);
		break;
	case SEROUT:
		adc.enable();
		break;
	}
}
