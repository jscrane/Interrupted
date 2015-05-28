#include <TinyDebugSerial.h>
#include <Interrupted.h>

TinyDebugSerial serial;
Analog adc(A1, vcc);
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
