#include <TinyDebugSerial.h>
#include <Interrupted.h>

TinyDebugSerial serial;
Analog adc(A3, vcc);
Devices devices;

void setup(void)
{
	devices.add(adc);
	devices.begin();

	pinMode(0, OUTPUT);
	serial.begin(115200);
	serial.println("hello world");
}

void loop(void)
{
	adc.wake();
	adc.enable(true);
	devices.select();
	serial.println(adc.read());
	adc.sleep();
}
