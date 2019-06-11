#include <Interrupted.h>

#define SEROUT	99

SerialOut<16> output(SEROUT, TERMINAL_SPEED);
Devices devices;

void setup(void) {
	devices.add(output);
	devices.begin();
	output.println("hello");
}

void loop(void) {
	if (devices.select() == SEROUT)
		output.println("hello world");
}
