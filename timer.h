#ifndef __TIMER_H__
#define __TIMER_H__

/**
 * A millisecond timer.
 */
class Timer: public Device {
public:
	Timer(int id, uint32_t millis): 
		Device(id), _delay(millis), _ticks(millis) {}

	void ready() {
		if (--_ticks == 0) {
			Device::ready();
			_ticks = _delay;
			disable();
		}
	}

	void delay(uint32_t d) { 
		noInterrupts(); 
		_ticks = _delay = d; 
		interrupts();
	}

	// not enabled by default
	bool begin();

protected:
	void _enable(bool);
	unsigned _sleepmode();

	uint32_t _delay;
	volatile uint32_t _ticks;
};

#endif
