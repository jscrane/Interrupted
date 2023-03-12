#include <TinyDebugSerial.h>
#include <Interrupted.h>

TinyDebugSerial serial;
Analog adc(A1);
Devices devices(adc);

void setup(void)
{
	devices.begin();

	serial.begin(115200);
	serial.println("hello world");
	adc.enable();
}

void loop(void)
{
	if (devices.select() == A1) {
		serial.println(adc.read());
		adc.enable();
	}
}
