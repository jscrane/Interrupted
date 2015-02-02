#ifndef __SERIALDEVICE_H__
#define __SERIALDEVICE_H__

class SerialDevice: public Device {
public:
	SerialDevice(unsigned long baud, unsigned id): 
		Device(id), _baud(baud) {}

	void begin();
	unsigned sleepmode();

private:
	unsigned long _baud;
};

#endif
