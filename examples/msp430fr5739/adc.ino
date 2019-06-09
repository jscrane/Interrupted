#include <Interrupted.h>

/*
 * LDR on A3
 */
#define SEROUT	99

Analog adc(A3);
SerialOut output(SEROUT, TERMINAL_SPEED);
Devices devices;

void setup(void)
{
	devices.add(adc);
	devices.add(output);
	devices.begin();

	pinMode(A3, INPUT_PULLUP);
	adc.enable();
}

void loop(void)
{
	char buf[8];
	
	switch (devices.select()) {
	case A3:
		itoa(adc.read(), buf, 10);
		strcat(buf, "\r\n");
		output.write(buf);
		break;
	case SEROUT:
		adc.enable();
		break;
	}
}
