#ifndef __DEVICE_H__
#define __DEVICE_H__

class Device;

class Devices {
public:
	Devices(Device &a) {
		_devices[_n++] = &a;
	}

	Devices(Device &a, Device &b) {
		_devices[_n++] = &a; _devices[_n++] = &b;
	}

	Devices(Device &a, Device &b, Device &c) {
		_devices[_n++] = &a; _devices[_n++] = &b;
		_devices[_n++] = &c;
	}

	Devices(Device &a, Device &b, Device &c, Device &d) {
		_devices[_n++] = &a; _devices[_n++] = &b;
		_devices[_n++] = &c; _devices[_n++] = &d;
	}

	Devices(Device &a, Device &b, Device &c, Device &d, Device &e) {
		_devices[_n++] = &a; _devices[_n++] = &b;
		_devices[_n++] = &c; _devices[_n++] = &d;
		_devices[_n++] = &e;
	}

	Devices(Device &a, Device &b, Device &c, Device &d, Device &e, Device &f) {
		_devices[_n++] = &a; _devices[_n++] = &b;
		_devices[_n++] = &c; _devices[_n++] = &d;
		_devices[_n++] = &e; _devices[_n++] = &f;
	}

	Devices(Device &a, Device &b, Device &c, Device &d, Device &e, Device &f, Device &g) {
		_devices[_n++] = &a; _devices[_n++] = &b;
		_devices[_n++] = &c; _devices[_n++] = &d;
		_devices[_n++] = &e; _devices[_n++] = &f;
		_devices[_n++] = &g;
	}

	void begin();

	int select();

	static const unsigned SLEEP_MODE_NONE = 0xabcd;

	static unsigned compare_modes(unsigned sys, unsigned dev);

	static void powersave();

protected:
	virtual void idle(unsigned mode) {
		sleep(mode);
	}

	static void sleep(unsigned mode);

private:
	int _n;
	Device *_devices[7];
};

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

	void enable(bool enabled=true) { 
		if (_enabled != enabled) {
			_enabled = enabled; 
			_enable(enabled); 
		}
	}

	bool is_enabled() { return _enabled; }

	int id() { return _id; }

	inline unsigned negotiate_mode(unsigned sys) {
		return Devices::compare_modes(sys, _sleepmode());
	}

protected:
	Device(int id): _id(id), _ready(false) {}

	virtual void _enable(bool) = 0;
	virtual unsigned _sleepmode();

private:
	int _id;
	volatile bool _enabled;
	volatile bool _ready;
};

#endif
