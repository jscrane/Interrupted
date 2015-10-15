#ifndef __DELAY_H__
#define __DELAY_H__

/**
 * A millisecond timer, based on the platform's millisecond clock.
 */
class Delay: public Device {
public:
	Delay(int id, uint32_t delay): Device(id), _delay(delay) {}

	// not enabled by default
	bool begin() { return false; }

	bool is_ready() {
		if (!is_enabled() || millis() < _wake)
			return false;
		disable();
		return true;
	}

protected:
	void _enable(bool e) {
		if (e)
			_wake = millis() + _delay;
	}
	
	unsigned _sleepmode();

private:
	uint32_t _delay, _wake;
};

#endif
