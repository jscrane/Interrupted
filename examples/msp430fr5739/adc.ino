#include <Interrupted.h>

/*
 * LDR on A3
 */
#define SEROUT	99

Analog adc(A3);
SerialOut<> output(SEROUT, TERMINAL_SPEED);
Devices devices(adc, output);

void setup(void)
{
	devices.begin();

	pinMode(A3, INPUT_PULLUP);
	adc.enable();
}

void loop(void)
{
	switch (devices.select()) {
	case A3:
		output.println(adc.read());
		break;
	case SEROUT:
		adc.enable();
		break;
	}
}
