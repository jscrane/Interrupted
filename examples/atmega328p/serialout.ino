#include <Interrupted.h>
#include <avr/wdt.h>

/*
 * Serial output running at 115200 baud.
 *
 * The Watchdog timer fires every 2s and writes a message.
 */
#define TIMER	1
#define SEROUT	99

SerialOut<> output(SEROUT, TERMINAL_SPEED);
Watchdog timer(TIMER, WDTO_2S);
Devices devices(timer, output);

void setup() {
	devices.begin();
	timer.enable();
}

void loop() {
	switch (devices.select()) {
	case TIMER:
		timer.disable();
		output.println("hello world");
		break;
	case SEROUT:
		timer.enable();
		break;
	}
}
