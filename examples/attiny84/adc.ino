#include <Interrupted.h>

/*
 * An analog input on pin 10, ADC3 (e.g., LDR/resistor voltage divider).
 * The input is read and written to Serial at 19200 baud.
 */
Analog adc(A3, EXTERNAL);
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
	if (devices.select() == A3)
		Serial.println(adc.read());
}
