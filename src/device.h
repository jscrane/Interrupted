#ifndef __DEVICE_H__
#define __DEVICE_H__

#ifndef MAX_DEVICES
#define MAX_DEVICES 10
#endif

class Device;

#define LOW_POWER	true

class Devices {
public:
	Devices(): _n(0) {}

	void add(Device &device) {
		if (_n < MAX_DEVICES)
			_devices[_n++] = &device;
	}

	// if powersave is set, turn off all devices at startup
	void begin(bool powersave = false);

	int select();

	static const unsigned SLEEP_MODE_NONE = 0xabcd;

	static unsigned compare_modes(unsigned sys, unsigned dev);

protected:
	virtual void idle(unsigned mode) {
		sleep(mode);
	}

	static void sleep(unsigned mode);
private:
	int _n;
	Device *_devices[MAX_DEVICES];
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
