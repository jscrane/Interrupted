#ifndef __ATIMER_H__
#define __ATIMER_H__

/**
 * An abstract one-shot timer. 
 */
class AbstractTimer: public Device {
public:
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

protected:
	AbstractTimer(int id, uint32_t delay): 
		Device(id), _delay(delay), _ticks(delay) {}

private:
	uint32_t _delay;
	volatile uint32_t _ticks;
};

#endif
