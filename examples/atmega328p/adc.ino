#include <Interrupted.h>

/*
 * An analog input on A0 (e.g., an LDR/resistor voltage divider).
 * Serial output running at 115200 baud.
 *
 * The Watchdog timer fires every 1s and kicks off an ADC. When
 * this is ready, the Serial output is enabled and the value written.
 * When output is done, it is disabled and the watchdog re-enabled.
 */
#define TIMER	1
#define SEROUT	99

Analog adc(A0);
SerialOut<> output(SEROUT, TERMINAL_SPEED);
Watchdog timer(TIMER);
Devices devices;

void setup() {
	devices.add(adc);
	devices.add(timer);
	devices.add(output);
	devices.begin();
	timer.enable();
}

void loop() {
	switch (devices.select()) {
	case TIMER:
		adc.enable();
		break;
	case A0:
		output.println(adc.read());
		break;
	case SEROUT:
		timer.enable();
		break;
	}
}
