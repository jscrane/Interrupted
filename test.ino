#include "device.h"
#include "external.h"
#include "pinchange.h"
#include "timer.h"
#include "timer1.h"
#include "watchdog.h"
#include "serial.h"
#include "serialin.h"
#include "serialout.h"

SerialIn input(98, 115200);
SerialOut output(99);
Watchdog timer(5, 1);
External int0(2), int1(3, RISING);
PinChangeGroup pins(D8_13);
PinChange led(pins, 13);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(int0);
	devices.add(int1);
	devices.add(led);
	devices.add(output);
	devices.add(input);
	devices.begin();

	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
}

void loop(void)
{
	char c;

	switch (devices.select()) {
	case 2:
		digitalWrite(13, HIGH);	// external interrupt #0
		break;
	case 1:
	case 3:
		digitalWrite(13, LOW);	// timer or ext interrupt #1
		break;
	case 13:
		int0.enable(!led.is_on());
		int1.enable(led.is_on());
		timer.enable(led.is_on());
		break;
	case 98:			// serial char received
		c = input.read();
		if (c == '1')
			digitalWrite(13, HIGH);
		else if (c == '0')
			digitalWrite(13, LOW);
		return;
	case 99:
		return;			// serial transmission complete
	}

	output.write("awake!\r\n");
}
