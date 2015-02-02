#ifndef __SERIAL_DEVICE_H__
#define __SERIAL_DEVICE_H__

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
