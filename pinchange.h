#ifndef __PINCHANGE_H__
#define __PINCHANGE_H__

class Pin;

class Port {
public:
	Port(): _port(0), _enabled(0) {}

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
	byte _port, _enabled;
};

class Pin: public Device {
public:
	Pin(int pin, Port &port, byte s = LOW): Device(pin), _port(port), _state(s == HIGH) {}

	bool begin() { _port.add_pin(id(), this); return true; }
	void set_state(bool s) {
		if (_state != s) { _state = s; ready(); }
	}
	bool is_on() { return _state; }
	bool is_high() { return is_on(); }
	bool is_low() { return !is_on(); }

protected:
	void _enable(bool enable) { _port.enable_pin(id(), enable); }

private:
	Port &_port;
	volatile bool _state;
};

#endif
