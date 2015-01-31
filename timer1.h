#ifndef __TIMER1_H__
#define __TIMER1_H__

/**
 * A millisecond timer.
 */
class Timer1: public Timer {
public:
	Timer1(unsigned millis, unsigned id): Timer(millis, id) {}

	void begin();
	void enable(bool enable = true);
	unsigned sleepmode();
};

#endif
