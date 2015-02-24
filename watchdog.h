#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/**
 * A Watchdog timer. Its resolution defaults to 1s. (See avr/wdt.h)
 */
class Watchdog: public Timer {
public:
	Watchdog(int id, unsigned n, unsigned scale=-1): Timer(id, n), _scale(scale) {}

	void begin();

protected:
	void _enable(bool enable);

private:
	unsigned _scale;
};

#endif
