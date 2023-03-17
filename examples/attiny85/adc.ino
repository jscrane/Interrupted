/*
 * An analog input on pin 7, A1 (e.g., LDR/resistor voltage divider).
 * Input is read and written to Serial at TERMINAL_SPEED.
 */
#include <Interrupted.h>

Analog adc(A1);
Devices devices(adc);

void setup(void)
{
	Serial.begin(TERMINAL_SPEED);
	Serial.println("hello world");

	devices.begin();
}

void loop(void)
{
	adc.enable();
	if (devices.select() == A1)
		Serial.println(adc.read());
}
