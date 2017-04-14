#ifndef __SERIALDEVICE_H__
#define __SERIALDEVICE_H__

#include "device.h"

class SerialDevice: public Device {
public:
	SerialDevice(unsigned id, unsigned long baud):
		Device(id), _awake(false), _baud(baud) {}

	void init();
	virtual void sleep();
	virtual void wake();

	virtual bool is_awake() {return _awake; }

protected:
	bool _awake;

private:
	unsigned _sleepmode();
	unsigned long _baud;
};

#endif
