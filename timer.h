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
			_ticks = _delay;
			enable(false);
		}
	}

	void delay(unsigned d) { 
		noInterrupts(); 
		_ticks = _delay = d; 
		interrupts();
	}

protected:
	Timer(int id, unsigned delay): 
		Device(id), _delay(delay), _ticks(delay) {}

private:
	uint32_t _delay;
	volatile uint32_t _ticks;
};

#endif
