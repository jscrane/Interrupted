#include <TinyDebugSerial.h>
#include <Interrupted.h>

/*
 * An analog input on pin 10, ADC3 (e.g., LDR/resistor voltage divider).
 * The input is read and written to TinyDebugSerial output at 115200 baud.
 */
TinyDebugSerial serial;
Analog adc(A3, vcc);
Devices devices;

void setup(void)
{
	devices.add(adc);
	devices.begin();

	serial.begin(115200);
	serial.println("hello world");
}

void loop(void)
{
	adc.wake();
	adc.enable();
	devices.select();
	serial.println(adc.read());
	adc.sleep();
}
