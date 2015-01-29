#ifndef __DEVICE_H__
#define __DEVICE_H__

class Device {
public:
	virtual void begin() =0;

	bool ready();

	virtual void trigger() { _triggered = true; }

	virtual void enable(bool enable) { _enabled = enable; }

	int id() { return _id; }

protected:
	Device(int id): _id(id), _enabled(true), _triggered(false) {}

private:
	int _id;
	bool _enabled;
	volatile bool _triggered;
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
