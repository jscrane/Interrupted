#include <Interrupted.h>

#define SEROUT	99

SerialOut output(SEROUT, 9600);
Devices devices;

void setup(void) {
	devices.add(output);
	devices.begin();
}

void loop(void) {
	output.write("hello world\r\n");
	while (devices.select() != SEROUT);
}
