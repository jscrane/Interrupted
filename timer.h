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
		if (_ticks <= 1) {
			Device::ready();
			_ticks = _delay;
			disable();
		} else
			_ticks--;
	}

	void delay(uint32_t d) {
		cli();
		_ticks = _delay = d;
		sei();
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
