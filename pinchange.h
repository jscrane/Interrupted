#ifndef __PINCHANGE_H__
#define __PINCHANGE_H__

typedef enum { D8_13, D0_7, A0_5 } pinchange_t;

class PinChange;

class PinChangeGroup {
public:
	PinChangeGroup(pinchange_t group): 
		_group(group), _enabled(0), _state(0) {}

	void add_pin(PinChange *p, int pin);
	void enable_pin(int pin, boolean enable);

	void ready();

private:
	PinChange *_pins[8];
	int port_offset();
	pinchange_t _group;
	byte _enabled;
	volatile byte _state;
};

class PinChange: public Device {
public:
	PinChange(int pin, PinChangeGroup &group): 
		Device(pin), _group(group) {}

	void enable(bool enable = true) {
		Device::enable(enable);
		_group.enable_pin(id(), enable);
	}

	void begin() { _group.add_pin(this, id()); }
	void set_state(bool on) { _state = on; ready(); }
	bool is_on() { return _state; }

private:
	PinChangeGroup &_group;
	bool _state;
};

#endif
