#ifndef __DEVICE_H__
#define __DEVICE_H__

class Device {
public:
	// devices which are enabled at startup return true here
	virtual bool begin() =0;

	virtual void ready() { 
		if (_enabled) 
			_ready = true; 
	}

	virtual bool is_ready() {
		if (_ready) {
			_ready = false;
			return true;
		}
		return false;
	}

	void disable() { enable(false); }
	void enable(bool enabled=true) { _enabled = enabled; _enable(enabled); }
	bool is_enabled() { return _enabled; }

	int id() { return _id; }
	unsigned negotiate_mode(unsigned);

protected:
	Device(int id): _id(id), _ready(false) {}

	virtual void _enable(bool) = 0;
	virtual unsigned _sleepmode();

private:
	int _id;
	bool _enabled;
	volatile bool _ready;
};

#define MAX_DEVICES 10

class Devices {
public:
	Devices(): _n(0) {}

	void add(Device &device) {
		if (_n < MAX_DEVICES)
			_devices[_n++] = &device;
	}

	void begin();
	int select();

private:
	int _n;
	Device *_devices[MAX_DEVICES];
};

#endif
