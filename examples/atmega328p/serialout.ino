#include <Interrupted.h>
#include <avr/wdt.h>

/*
 * Serial output running at 115200 baud.
 *
 * The Watchdog timer fires every 2s and writes a message.
 */
#define TIMER	1
#define SEROUT	99

SerialOut output(SEROUT, 115200);
Watchdog timer(TIMER, WDTO_2S);
Devices devices;

void setup() {
	devices.add(timer);
	devices.add(output);
	devices.begin();
	timer.enable();
}

void loop() {
	switch (devices.select()) {
	case TIMER:
		timer.disable();
		output.write("hello world\r\n");
		break;
	case SEROUT:
		output.disable();
		timer.enable();
		break;
	}
}
