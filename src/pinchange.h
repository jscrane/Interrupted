#ifndef __PINCHANGE_H__
#define __PINCHANGE_H__

class Ports {
public:
	static void register_pin(uint8_t pin, class Pin *p);
	static void enable_pin(uint8_t pin, bool enable);

	static void ready(uint8_t port);
private:
	static uint8_t bit_index(uint8_t b) {
		uint8_t i = 0;
		while (b >>= 1)
			i++;
		return i;
	}
};

class Pin: public Device {
public:
	Pin(int pin, uint8_t s = LOW): Device(pin), _state(s == HIGH) {}

	bool begin() { Ports::register_pin(id(), this); return true; }
	void set_state(bool s) {
		if (_state != s) { _state = s; ready(); }
	}
	bool is_on() { return _state; }
	bool is_high() { return is_on(); }
	bool is_low() { return !is_on(); }

protected:
	void _enable(bool enable) { Ports::enable_pin(id(), enable); }

private:
	volatile bool _state;
};

#endif
