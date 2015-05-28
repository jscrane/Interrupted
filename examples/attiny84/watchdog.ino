#include <avr/wdt.h>
#include <Interrupted.h>

/*
 * An LED on pin 2 (PB0, D10),
 * A pushbutton on pin 5 (PB2, INT0, D8),
 * An analog input on pin 10 (PA3, ADC3, e.g., an LDR/resistor voltage divider).
 * 
 * When the button is pressed, the LED comes on and remains on for a
 * time proportional to the ADC value (e.g., the light level).
 */
#define LED	10
#define TIMER	1
#define BUTTON	8
#define ADC	A3

PinChangeGroup porta;
PinChange led(LED, porta); 
Watchdog timer(TIMER, 1);
External button(BUTTON, LOW);
Devices devices;
Analog adc(ADC, vcc);

void setup(void)
{
	devices.add(timer);
	devices.add(button);
	devices.add(led);
	devices.add(adc);
	devices.begin();

	pinMode(LED, OUTPUT);
}

void loop(void)
{
	unsigned val = 0;

	switch (devices.select()) {
	case BUTTON:
		digitalWrite(LED, HIGH);
		break;
	case LED:
		adc.enable(led.is_on());
		break;
	case ADC:
		val = adc.read();
		timer.delay(val / 50);
		timer.enable(led.is_on());
		break;
	case TIMER:
		digitalWrite(LED, LOW);
		break;
	}
}
