#ifndef __TIMER_H__
#define __TIMER_H__

/**
 * An abstract timer.
 */
class Timer: public Device {
public:
	void ready() {
		if (--_remain == 0) {
			Device::ready();
			_remain = _dt;
		}
	}

	void reset(unsigned dt) {
		cli();
		_dt = _remain = dt;
		sei();
	}

protected:
	Timer(int id, unsigned dt): 
		Device(id), _dt(dt), _remain(dt) {}

private:
	unsigned _dt;
	volatile unsigned _remain;
};

#endif
