/*
 * Set IDE to "ATtiny84@1MHz, BOD disabled"
 * (Can program with Arduino as ISP using one of these:
 * http://programmablehardware.blogspot.ie/2011/05/in-system-programming.html )
 */
#include <stdint.h>
#include <math.h>
#include <avr/wdt.h>

#include <Interrupted.h>

// thermistor parameters
const double r0 = 10000;
const double beta = 3977.0;

// bias resistor
const double rb = 9990;
const double zeroC = 273.15;
const double tzero = 25 + zeroC;

typedef enum { cent, fahr, abso } units_t;

#define IDLE		10000L
#define DIGIT		500L
#define DEBOUNCE	250L

#define THERMISTOR	7
#define SWITCH		1
#define DIVIDER_GND 	2
#define A_PIN 		4
#define TIMER 		0

Analog thermistor(THERMISTOR, vcc);
PinChangeGroup pins(PB);
PinChange button(SWITCH, pins);
Timer1 timer(TIMER, DIGIT);
Devices devices;

const uint8_t A = _BV(0);
const uint8_t B = _BV(1);
const uint8_t C = _BV(2);
const uint8_t D = _BV(3);
const uint8_t E = _BV(4);
const uint8_t F = _BV(5);
const uint8_t G = _BV(6);

static const uint8_t digits[] = {
	A | B | C | D | E | F,
	B | C,
	A | B | G | E | D,
	A | B | G | C | D,
	F | G | B | C,
	A | F | G | C | D,
	A | F | E | G | C | D,
	A | B | C,
	A | B | C | D | E | F | G,
	A | B | C | F | G,
};

static const uint8_t minus = G;
static const uint8_t centigrade = A | F | E | D;
static const uint8_t fahrenheit = A | F | E | G;
static const uint8_t absolute = A | B | C | E | F | G;

void bits(uint8_t bits)
{
	uint8_t b = 1;
	for (int i = 0; i < 7; i++, b <<= 1)
		digitalWrite(A_PIN + i, bits & b);
}

double read_temp() {
	double rt = rb * (1023.0 / thermistor.read() - 1);
	double rtk = 1.0 / tzero + log(rt / r0) / beta;	
	return 1.0 / rtk;
}

void format_temp(units_t units, double t, uint8_t *buf) {
	uint8_t u = centigrade;
	if (units == fahr)
		u = fahrenheit;
	else if (units == abso)
		u = absolute;
	*buf++ = u;
	if (units != abso) {
		t -= zeroC;
		if (units == fahr)
			t = t * 9.0 / 5.0 + 32;
	}
	if (t < 0) {
		t = -t;
		*buf++ = minus;
	}
	int temp = (int)(t + 0.5);
	bool first = true;
	for (int pow = 100; pow > 0; pow /= 10) {
		int dig = temp / pow;
		if (!first || dig != 0 || pow == 1) {
			temp -= dig * pow;
			*buf++ = digits[dig];
			first = false;
		}
	}
	*buf++ = 0;
}

uint8_t *reformat(units_t units, double t, uint8_t *buf) {
	format_temp(units, t, buf);
	bits(buf[0]);
	return buf+1;
}

void setup(void)
{
	devices.add(thermistor);
	devices.add(button);
	devices.add(timer);
	devices.begin();
	
	for (int i = 0; i < 7; i++)
		pinMode(A_PIN + i, OUTPUT);
	
	pinMode(DIVIDER_GND, OUTPUT);
	digitalWrite(DIVIDER_GND, LOW);

	thermistor.enable();
}

void loop(void)
{
	static units_t units = cent;
	static bool sleeping = false; 
	static uint8_t buf[8], *p = buf;
	static double curr_temp = 0.0;
	static uint32_t last;
	uint32_t now = millis();

	switch (devices.select()) {
	case SWITCH:
		if (sleeping) {
			sleeping = false;
			last = now;
			pinMode(DIVIDER_GND, OUTPUT);
			digitalWrite(DIVIDER_GND, LOW);
			thermistor.wake();
			thermistor.enable();
		} else if (now - last > DEBOUNCE) {
			last = now;
			if (units == cent)
				units = fahr;
			else if (units == fahr)
				units = abso;
			else
				units = cent;
			p = reformat(units, curr_temp, buf);
		}
		break;
	case TIMER:
		if (*p) {
			bits(*p++);
			timer.enable();
		} else
			thermistor.enable();
		break;
	case THERMISTOR:
		curr_temp = read_temp();
		timer.enable();
		p = reformat(units, curr_temp, buf);
		break;
	}
	if (now - last > IDLE) {
		timer.enable(false);
		thermistor.enable(false);
		thermistor.sleep();
		pinMode(DIVIDER_GND, INPUT);
		bits(0);
		sleeping = true;
	}
}
