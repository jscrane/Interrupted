#include <Interrupted.h>

/*
 * Serial input running at TERMINAL_SPEED baud.
 * When a '1' is read the built-in LED is switched on, when anything else
 * is read, it's switched off.
 */
#define SERIN	99

SerialIn<2> input(SERIN, TERMINAL_SPEED);
Port portb;
Pin led(LED_BUILTIN, portb);
Devices devices(led, input);

void setup() {
	devices.begin();

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
	switch (devices.select()) {
	case LED_BUILTIN:
		input.enable();
		break;
	case SERIN:
		char c = input.read();
		input.disable();
		digitalWrite(LED_BUILTIN, c == '1');
		led.enable();
		break;
	}
}
