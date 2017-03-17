#include <Interrupted.h>

#define SEROUT	99

SerialOut output(SEROUT, 9600);
Devices devices;

void setup(void) {
	devices.add(output);
	devices.begin();
	output.write("hello\r\n");
}

void loop(void) {
	if (devices.select() == SEROUT)
		output.write("hello world\r\n");
}
