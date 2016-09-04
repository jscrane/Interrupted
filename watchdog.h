#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/**
 * A Watchdog timer. Its resolution defaults to 1s.
 */
class Watchdog: public Device {
public:
	Watchdog(int id, int scale = -1): Device(id), _scale(scale) {}

	void ready() {
		Device::ready();
		disable();
	}

	// not enabled by default
	bool begin();

	// change the prescaler, takes effect on next enable()
	void scale(unsigned scale) { _scale = scale; }

protected:
	void _enable(bool);
	unsigned _sleepmode();

private:
	int _scale;
};

#endif
