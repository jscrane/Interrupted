#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/**
 * A Watchdog timer; resolution in seconds.
 */
class Watchdog: public Device {
public:
	Watchdog(int id, unsigned secs=1): Device(id), _secs(secs), _left(0) {}

	void ready() {
		if (_left)
			_left = _update_prescaler(_left);
		else {
			Device::ready();
			disable();
		}
	}

	// not enabled by default
	bool begin();

	void delay(unsigned secs) { _secs = secs; }

protected:
	void _enable(bool);
	unsigned _sleepmode();

private:
	unsigned _update_prescaler(unsigned t);
	unsigned _secs;
	volatile unsigned _left;
};

#endif
