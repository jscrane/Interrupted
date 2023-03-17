/*
 * An analog input on pin 10, ADC3 (e.g., LDR/resistor voltage divider).
 * The input is read and written to Serial at 19200 baud.
 */
#include <math.h>
#include <Interrupted.h>

Analog thermistor(A3, EXTERNAL);
Devices devices(thermistor);

// thermistor parameters
const double r0 = 10000;
const double beta = 3977.0;

// bias resistor
const double rb = 9990;
const double zeroC = 273.15;
const double tzero = 25 + zeroC;

double celcius(int v) 
{
	double rt = rb * (1023.0 / v - 1);
	double rtk = 1.0 / tzero + log(rt / r0) / beta; 
	return 1.0 / rtk - zeroC;
}

void setup(void)
{
	Serial.begin(TERMINAL_SPEED);
	Serial.println("hello world");
	devices.begin();
}

void loop(void)
{
	thermistor.enable();
	if (devices.select() == A3)
		Serial.println(celcius(thermistor.read()));
	delay(1000);
}
