#include <Interrupted.h>

/*
 * Serial input running at 115200 baud. When a '1' is read and an LED on 
 * pin 13 is switched on. When anything else is read, it's switched off.
 */
#define LED	13
#define INPUT	99

SerialIn input(INPUT, 115200);
Port portb;
Pin led(LED, portb);
Devices devices;

void setup() {
	devices.add(led);
	devices.add(input);
	devices.begin();

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
}

void loop() {
	switch (devices.select()) {
	case LED:
		input.enable();
		break;
	case INPUT:
		char c = input.read();
		input.disable();
		digitalWrite(LED, c == '1');
		led.enable();
		break;
	}
}
