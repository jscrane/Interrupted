#ifndef __TIMER_H__
#define __TIMER_H__

/**
 * A millisecond timer.
 */
class Timer: public Device {
public:

	Timer(int id, uint32_t millis, uint32_t ms_divisor=1L):
		Device(id), _delay(millis), _ticks(millis), _ms_divisor(ms_divisor) {}


	void ready() {
		if (_ticks <= 1) {
			Device::ready();
			_ticks = _delay;
			disable();
		} else
			_ticks--;
	}

	void delay(uint32_t d) {
		noInterrupts();
		_ticks = _delay = d;
		interrupts();
	}

	// not enabled by default
	bool begin();

	// virtual void ready();

	virtual void sleep();
	virtual void wake();

protected:
	void _enable(bool);
	unsigned _sleepmode();

	uint32_t _delay;
	volatile uint32_t _ticks;
	uint32_t _ms_divisor;
};

#endif
