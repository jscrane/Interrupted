#ifndef __PINCHANGE_H__
#define __PINCHANGE_H__

class PinChange;

class PinChangeGroup {
public:
	PinChangeGroup(): _port(0), _enabled(0), _state(0) {}

	void add_pin(int pin, PinChange *p);
	void enable_pin(int pin, bool enable);

	void ready();

private:
	inline int bit_index(int b) {
		int i = 0;
		while (b >>= 1)
			i++;
		return i;
	}
	PinChange *_pins[8];
	byte _port;
	byte _enabled;
	volatile byte _state;
};

class PinChange: public Device {
public:
	PinChange(int pin, PinChangeGroup &group): Device(pin), _group(group) {}

	bool begin() { _group.add_pin(id(), this); return true; }
	void set_state(bool on) {
		if (_state != on) { _state = on; ready(); }
	}
	bool is_on() { return _state; }

protected:
	void _enable(bool enable) { _group.enable_pin(id(), enable); }

private:
	PinChangeGroup &_group;
	bool _state;
};

#endif
