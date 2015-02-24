#include <avr/wdt.h>
#include "device.h"
#include "external.h"
#include "pinchange.h"
#include "timer.h"
#include "timer1.h"
#include "watchdog.h"
#include "serial.h"
#include "serialin.h"
#include "serialout.h"
#include "adc.h"

#define LED	13
#define SER_IN	98
#define SER_OUT	99
#define EXT0	2
#define EXT1	3
#define TIMER	1

SerialIn input(SER_IN, 115200);
SerialOut output(SER_OUT);
Watchdog timer(TIMER, 1, WDTO_4S);
External int0(EXT0), int1(EXT1, RISING);
PinChangeGroup pins(PB);
PinChange led(LED, pins);
Analog adc(A0);
Devices devices;

void setup(void)
{
	devices.add(timer);
	devices.add(int0);
	devices.add(int1);
	devices.add(led);
	devices.add(output);
	devices.add(input);
	devices.add(adc);
	devices.begin();

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
}

void loop(void)
{
	char c, buf[8];

	switch (devices.select()) {
	case A0:
		output.enable(true);
		itoa(adc.read(), buf, 16);
		strcat(buf, "\r\n");
		output.write(buf);
		adc.enable(led.is_on());
		break;

	case EXT0:
		digitalWrite(13, HIGH);
		adc.enable(true);
		break;

	case TIMER:
	case EXT1:
		digitalWrite(13, LOW);
		break;

	case LED:
		int0.enable(!led.is_on());
		int1.enable(led.is_on());
		timer.enable(led.is_on());
		break;

	case SER_IN:
		c = input.read();
		if (c == '1')
			digitalWrite(LED, HIGH);
		else if (c == '0')
			digitalWrite(LED, LOW);
		break;

	case SER_OUT:
		output.enable(false);	// serial transmission complete
		break;
	}
}
