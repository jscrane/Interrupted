#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/**
 * A Watchdog timer. Its resolution defaults to 1s.
 */
class Watchdog: public AbstractTimer {
public:
	Watchdog(int id, unsigned ticks, unsigned scale = -1): 
		AbstractTimer(id, ticks), _scale(scale) {}

	// not enabled by default
	bool begin();

	// change the prescaler
	void scale(unsigned scale) { _scale = scale; _prescale(); }

protected:
	void _enable(bool);
	void _prescale();
	unsigned _sleepmode();

private:
	unsigned _scale;
};

#endif
