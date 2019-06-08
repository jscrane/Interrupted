#include <avr/wdt.h>
#include <Interrupted.h>

#define SER_IN	98
#define SER_OUT	99
#define EXT0	2
#define EXT1	3
#define TIMER	1

SerialIn input(SER_IN, 115200);
SerialOut output(SER_OUT);
Watchdog timer(TIMER, WDTO_4S);
External int0(EXT0), int1(EXT1, RISING);
Port portb;
Pin led(LED_BUILTIN, portb);
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

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
}

void loop(void)
{
	char c, buf[8];

	switch (devices.select()) {
	case A0:
		itoa(adc.read(), buf, 16);
		strcat(buf, "\r\n");
		output.write(buf);
		adc.enable(led.is_on());
		break;

	case EXT0:
		digitalWrite(13, HIGH);
		break;

	case TIMER:
	case EXT1:
		digitalWrite(13, LOW);
		break;

	case LED_BUILTIN:
		int0.enable(!led.is_on());
		int1.enable(led.is_on());
		timer.enable(led.is_on());
		adc.enable(led.is_on());
		break;

	case SER_IN:
		c = input.read();
		if (c == '1')
			digitalWrite(LED_BUILTIN, HIGH);
		else if (c == '0')
			digitalWrite(LED_BUILTIN, LOW);
		break;

	case SER_OUT:
		output.disable();	// serial transmission complete
		break;
	}
}
