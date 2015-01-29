#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__

class External: public Device {
public:
	External(int pin, int mode = FALLING): 
		Device(pin), _pin(pin), _mode(mode)  {}

	void begin();
	void enable(bool);

private:
	int _pin, _mode;
};
#endif
