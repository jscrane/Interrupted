#ifndef __TIMER_H__
#define __TIMER_H__

/**
 * A millisecond timer.
 */
class Timer: public AbstractTimer {
public:
	Timer(int id, uint32_t millis): AbstractTimer(id, millis) {}

	// not enabled by default
	bool begin();

protected:
	void _enable(bool);
	unsigned _sleepmode();
};

#endif
