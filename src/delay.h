#ifndef __DELAY_H__
#define __DELAY_H__

/**
 * A millisecond timer, based on the platform's millisecond clock.
 */
class Delay: public Device {
public:
	Delay(int id, uint32_t delay): Device(id), _delay(delay) {}

	// not enabled by default
	bool begin();

	bool is_ready() {
		if (is_enabled() && millis() >= _wake) {
			disable();
			return true;
		}
		return false;
	}
	uint32_t _wake;

protected:
	void _enable(bool e) {
		if (e && _wake < millis())
			_wake = millis() + _delay;
	}
	
	unsigned _sleepmode();

private:
	const uint32_t _delay;
};

#endif
