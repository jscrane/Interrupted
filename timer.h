#ifndef __TIMER_H__
#define __TIMER_H__

#include "atimer.h"

/**
 * A millisecond timer.
 */
class Timer: public AbstractTimer {
public:
	Timer(int id, uint32_t millis, uint32_t ms_divisor=1L):
		AbstractTimer(id, millis), _ms_divisor(ms_divisor) {}

	// not enabled by default
	bool begin();

	// virtual void ready();

	virtual void sleep();
	virtual void wake();

protected:
	void _enable(bool);
	unsigned _sleepmode();
	uint32_t _ms_divisor;
};

#endif
