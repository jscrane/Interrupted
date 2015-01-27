#ifndef __DEVICE_H__
#define __DEVICE_H__

class Device {
public:
	virtual void begin() =0;
	virtual bool ready() =0;

	bool enabled() { return _enabled; }
	void enable(bool e) { _enabled = e; }

	int id() { return _id; }

protected:
	Device(int id): _id(id), _enabled(true) {}

private:
	int _id;
	bool _enabled;
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
