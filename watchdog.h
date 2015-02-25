#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/**
 * A Watchdog timer. Its resolution defaults to 1s. (See avr/wdt.h)
 */
class Watchdog: public Timer {
public:
	Watchdog(int id, unsigned ticks, unsigned scale = -1): 
		Timer(id, ticks), _scale(scale) {}

	// not enabled by default
	bool begin();

	// change the prescaler
	void scale(unsigned scale) { _scale = scale; _prescale(); }

protected:
	void _enable(bool);
	void _prescale();

private:
	unsigned _scale;
};

#endif
