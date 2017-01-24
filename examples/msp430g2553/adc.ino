#include <Interrupted.h>

/*
 * An analog input on pin 6, A4 (e.g., LDR/resistor voltage divider).
 */
Analog adc(A4, INTERNAL2V5);
Devices devices;

void setup(void)
{
	devices.add(adc);
	devices.begin();

	Serial.begin(9600);
}

void loop(void)
{
	adc.wake();
	adc.enable();
	if (devices.select() == A4)
		Serial.println(adc.read());
	adc.sleep();
}
