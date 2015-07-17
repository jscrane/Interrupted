#include <Interrupted.h>

#define SEROUT	99

SerialOut output(SEROUT, 2400);
Devices devices;

void setup(void) {
	devices.add(output);
	devices.begin();
	output.enable();
}

void loop(void) {
	output.write("hello world\r\n");
	devices.select();
}
