#ifndef __SERIALDEVICE_H__
#define __SERIALDEVICE_H__

class SerialDevice: public Device {
public:
	SerialDevice(unsigned id, unsigned long baud): 
		Device(id), _baud(baud) {}

	void init();
private:
	unsigned _sleepmode();
	unsigned long _baud;
};

#endif
