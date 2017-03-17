#include <Interrupted.h>

/*
 * LDR on A3
 */
Analog adc(A3);
Devices devices;

void setup(void)
{
	devices.add(adc);
	devices.begin();

	Serial.begin(9600);
	pinMode(A3, INPUT_PULLUP);
	adc.enable();
}

void loop(void)
{
	if (devices.select() == A3) {
		Serial.println(adc.read());
		adc.enable();
	}
}
