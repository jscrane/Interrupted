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
	void pin(int pin) { _pin = pin; _mux(); }

	// changes the voltage reference
	void ref(unsigned ref) { _ref = ref; _mux(); }

	// returns last converted value or 0xffff if not ready
	unsigned read();

	// call to turn off ADC altogether and back on again
	void sleep();
	void wake();

protected:
	void _enable(bool enabled);

private:
	void _mux();
	unsigned _sleepmode();

	int _pin;
	unsigned _ref;
};

#endif
