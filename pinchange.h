#ifndef __PINCHANGE_H__
#define __PINCHANGE_H__

#include <avr/io.h>
#include "device.h"

class Pin;

class Port {
public:
	Port(): _port(0), _enabled(0), _state(0) {}

	void add_pin(int pin, Pin *p);
	void enable_pin(int pin, bool enable);

	void ready();

private:
	inline int bit_index(int b) {
		int i = 0;
		while (b >>= 1)
			i++;
		return i;
	}
	Pin *_pins[8];
	uint8_t _port;
	uint8_t _enabled;
	volatile uint8_t _state;
};

class Pin: public Device {
public:
	Pin(int pin, Port &port): Device(pin), _port(port) {}

	bool begin() { _port.add_pin(id(), this); return true; }
	void set_state(bool on) {
		if (_state != on) { _state = on; ready(); }
	}
	bool is_on() { return _state; }

protected:
	void _enable(bool enable) { _port.enable_pin(id(), enable); }

private:
	Port &_port;
	bool _state;
};

#endif
