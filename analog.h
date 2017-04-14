#ifndef __ANALOG_H__
#define __ANALOG_H__

#include <Arduino.h>
#include "device.h"

/**
 * Analog-to-Digital conversion.
 *
 * FIXME: analogReadResolution()
 */
class Analog: public Device {
public:
	// pin is the analog input, e.g., A0-A7
	Analog(int pin, unsigned ref = DEFAULT):
		Device(pin), _pin(pin), _ref(ref), _next_reading_valid(false) {}

	// not enabled by default
	bool begin();

	// changes the analog input
	void pin(int pin) { _pin = pin; _init(); }

	// changes the voltage reference
	void ref(unsigned ref) { _ref = ref; _init(); }

	// returns last converted value or 0xffff if not ready
	unsigned read();

	// call to turn off ADC altogether and back on again
	virtual void sleep();
	virtual void wake();

	bool next_reading_valid(void) {return _next_reading_valid;}
	void next_reading_will_be_valid() {_next_reading_valid = true; }

protected:
	void _enable(bool enabled);

private:
	void _init();
	unsigned _sleepmode();

	int _pin;
	unsigned _ref;
	volatile bool _next_reading_valid;
};

#endif
