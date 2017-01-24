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
}

void loop(void)
{
	//Serial.println(analogRead(A4));
	adc.wake();
	adc.enable();
	if (devices.select() == A3)
		Serial.println(adc.read());
	adc.sleep();
}
