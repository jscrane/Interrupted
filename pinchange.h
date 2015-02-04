#ifndef __PINCHANGE_H__
#define __PINCHANGE_H__

typedef enum { PA, PB, PC, PD } pinchange_port_t;

class PinChange;

class PinChangeGroup {
public:
	PinChangeGroup(pinchange_port_t port): 
		_port(port), _enabled(0), _state(0) {}

	void add_pin(int pin, PinChange *p);
	void enable_pin(int pin, boolean enable);

	void ready();

private:
	PinChange *_pins[8];
	int port_bit(int pin);
	pinchange_port_t _port;
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

	void begin() { _group.add_pin(id(), this); }
	void set_state(bool on) { _state = on; ready(); }
	bool is_on() { return _state; }

private:
	PinChangeGroup &_group;
	bool _state;
};

#endif
