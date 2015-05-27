#ifndef __TIMER1_H__
#define __TIMER1_H__

/**
 * A millisecond timer.
 */
class Timer1: public Timer {
public:
	Timer1(int id, uint32_t millis): Timer(id, millis) {}

	// not enabled by default
	bool begin();

protected:
	void _enable(bool);
	unsigned _sleepmode();
};

#endif
