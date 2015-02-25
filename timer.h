#ifndef __TIMER_H__
#define __TIMER_H__

/**
 * An abstract one-shot timer. 
 */
class Timer: public Device {
public:
	void ready() {
		if (--_ticks == 0) {
			Device::ready();
			enable(false);
		}
	}

	void delay(unsigned d) { _delay = d; }

protected:
	Timer(int id, unsigned delay): 
		Device(id), _delay(delay), _ticks(delay) {}

private:
	unsigned _delay;

	volatile unsigned _ticks;
};

#endif
