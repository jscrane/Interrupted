#ifndef __TIMER_H__
#define __TIMER_H__

/**
 * An abstract timer.
 */
class Timer: public Device {
public:
	void ready() {
		if (--_curr == 0) {
			Device::ready();
			_curr = _dt;
		}
	}
	
protected:
	Timer(unsigned dt, unsigned id): 
		Device(id), _dt(dt), _curr(dt) {}

private:
	unsigned _dt, _curr;
};

#endif
