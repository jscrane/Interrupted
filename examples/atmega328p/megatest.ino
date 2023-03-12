#include <avr/wdt.h>
#include <Interrupted.h>

#define SERIN	98
#define SEROUT	99
#define EXT0	2
#define EXT1	3
#define TIMER	1

SerialIn<2> input(SERIN, TERMINAL_SPEED);
SerialOut<> output(SEROUT);
Watchdog timer(TIMER, WDTO_4S);
External int0(EXT0), int1(EXT1, RISING);
Port portb;
Pin led(LED_BUILTIN, portb);
Analog adc(A0);
Devices devices(timer, int0, int1, led, output, input, adc);

void setup(void)
{
	devices.begin();

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
}

void loop(void)
{
	switch (devices.select()) {
	case A0:
		output.println(adc.read());
		adc.enable(led.is_on());
		break;

	case EXT0:
		digitalWrite(LED_BUILTIN, HIGH);
		break;

	case TIMER:
	case EXT1:
		digitalWrite(LED_BUILTIN, LOW);
		break;

	case LED_BUILTIN:
		int0.enable(!led.is_on());
		int1.enable(led.is_on());
		timer.enable(led.is_on());
		adc.enable(led.is_on());
		break;

	case SERIN:
		digitalWrite(LED_BUILTIN, input.read() == '1');
		break;

	case SEROUT:
		output.disable();	// serial transmission complete
		break;
	}
}
