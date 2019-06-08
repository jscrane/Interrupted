#include <Interrupted.h>

/*
 * Serial input running at 115200 baud. When a '1' is read and built-in LED
 * on  pin 13 is switched on. When anything else is read, it's switched off.
 */
#define INPUT	99

SerialIn input(INPUT, 115200);
Port portb;
Pin led(LED_BUILTIN, portb);
Devices devices;

void setup() {
	devices.add(led);
	devices.add(input);
	devices.begin();

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
	switch (devices.select()) {
	case LED_BUILTIN:
		input.enable();
		break;
	case INPUT:
		char c = input.read();
		input.disable();
		digitalWrite(LED_BUILTIN, c == '1');
		led.enable();
		break;
	}
}
