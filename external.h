#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__

class External: public Device {
public:
	External(int pin): Device(pin), _pin(pin)  {}

	void begin();

	void trigger() {
		_triggered = true;
	}

	bool ready() {
		bool t = _triggered;
		_triggered = false;
		return t;
	}

private:
	volatile bool _triggered;
	int _pin;
};
#endif
