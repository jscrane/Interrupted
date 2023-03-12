#include <Interrupted.h>

#define SEROUT	99

SerialOut<> output(SEROUT, TERMINAL_SPEED);
Devices devices(output);

void setup(void) {
	devices.begin();
}

void loop(void) {
	output.println("hello world");
	while (devices.select() != SEROUT);
}
