#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/**
 * A Watchdog timer. Its resolution defaults to 1s. (See avr/wdt.h)
 */
class Watchdog: public Timer {
public:
	Watchdog(int id, unsigned n, unsigned scale=-1): 
		Timer(id, n), _scale(scale) {}

	// not enabled by default
	bool begin();

protected:
	void _enable(bool);

private:
	unsigned _scale;
};

#endif
