#include <Interrupted.h>

/*
 * An analog input on pin 6, A4 (e.g., LDR/resistor voltage divider).
 *
 * For the serial port to work properly on Linux (3.11):
 * - program the launchpad
 * - unplug it
 * - press and hold the reset button
 * - plug launchpad back in
 * - wait for permissions on /dev/ttyACM0 to settle
 * - run "minicom -b 9600 -D /dev/ttyACM0"
 * - release reset button
 */
Analog adc(A4);
Devices devices;

void setup(void)
{
	devices.add(adc);
	devices.begin();

	Serial.begin(9600);
	Serial.println("hello world");
}

void loop(void)
{
	adc.wake();
	adc.enable();
	devices.select();
	Serial.println(adc.read());
	adc.sleep();
}
