#include <math.h>
#include <TinyDebugSerial.h>
#include <Interrupted.h>

TinyDebugSerial serial;
Analog thermistor(A7, vcc);
Devices devices;

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
	devices.add(thermistor);
	devices.begin();

	pinMode(0, OUTPUT);
	serial.begin(115200);
	serial.println("hello world");

	for (int i = 4; i < 11; i++)
		pinMode(i, OUTPUT);
}

void loop(void)
{
	thermistor.enable();
	devices.select();
	serial.println(celcius(thermistor.read()));
	delay(1000);
}
