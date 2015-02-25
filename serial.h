#ifndef __SERIALDEVICE_H__
#define __SERIALDEVICE_H__

class SerialDevice: public Device {
public:
	SerialDevice(unsigned id, unsigned long baud): 
		Device(id), _baud(baud) {}

	void init();
	unsigned sleepmode();

private:
	unsigned long _baud;
};

#endif
