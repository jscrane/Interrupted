#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__

class External: public Device {
public:
	External(int pin): Device(pin), _pin(pin)  {}

	void begin();

private:
	int _pin;
};
#endif
