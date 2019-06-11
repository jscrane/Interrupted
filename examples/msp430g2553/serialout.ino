#include <Interrupted.h>

#define SEROUT	99

SerialOut<> output(SEROUT, TERMINAL_SPEED);
Devices devices;

void setup(void) {
	devices.add(output);
	devices.begin();
}

void loop(void) {
	output.println("hello world");
	while (devices.select() != SEROUT);
}
