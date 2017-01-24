#include <Interrupted.h>

#define SERIN	98

SerialIn input(SERIN, 9600);
Devices devices;

void setup(void) {
	devices.add(input);
	devices.begin();
	pinMode(RED_LED, OUTPUT);
	digitalWrite(RED_LED, LOW);
}

void loop(void) {
	if (devices.select() == SERIN)
		digitalWrite(RED_LED, input.read() == '1'? HIGH: LOW);
}
