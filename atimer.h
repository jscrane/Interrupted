#ifndef __ATIMER_H__
#define __ATIMER_H__

// #include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "device.h"

/**
 * An abstract one-shot timer.
 */
class AbstractTimer: public Device {
public:
	void ready() {
		if (_ticks <= 1) {
			Device::ready();
			_ticks = _delay;
			disable();
		}
		else
		{
			--_ticks;
		}
	}

	void delay(uint32_t d) {
		cli();
		_ticks = _delay = d;
		sei();
	}

protected:
	AbstractTimer(int id, uint32_t delay):
		Device(id), _delay(delay), _ticks(delay) {}

private:
	uint32_t _delay;
	volatile uint32_t _ticks;
};

#endif
