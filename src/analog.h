#ifndef __ANALOG_H__
#define __ANALOG_H__

/**
 * Analog-to-Digital conversion.
 * 
 * FIXME: analogReadResolution()
 */
class Analog: public Device {
public:
	// pin is the analog input, e.g., A0-A7
	Analog(int pin, unsigned ref = DEFAULT):
		Device(pin), _pin(pin), _ref(ref) {}

	// not enabled by default
	bool begin();

	// changes the analog input
	void pin(int pin) { _pin = pin; _init(); }

	// changes the voltage reference
	void ref(unsigned ref) { _ref = ref; _init(); }

	// returns last converted value or 0xffff if not ready
	unsigned read();

protected:
	void _enable(bool enabled);

private:
	void _init();
	unsigned _sleepmode();

	int _pin;
	unsigned _ref;
};

#endif
