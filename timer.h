#ifndef __TIMER_H__
#define __TIMER_H__

// a simple timer with 1s resolution
class Timer: public Device {
public:
	void ready();
	
protected:
	Timer(unsigned secs, unsigned id): 
		Device(id), _secs(secs), _curr(secs) {}

private:
	unsigned _secs, _curr;
};

class Watchdog: public Timer {
public:
	Watchdog(unsigned secs, unsigned id): Timer(secs, id) {}

	void begin();
	void enable(bool enable = true);
};

#endif
