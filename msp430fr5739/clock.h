#ifndef __CLOCK_H__
#define __CLOCK_H__

class Clock: public Device {
public:
	Clock(int id): Device(id) {}

	void set(uint8_t h, uint8_t m, uint8_t s);

	// enabled by default
	bool begin();

	uint8_t hour() { return _h; }
	uint8_t mins() { return _m; }
	uint8_t secs() { return _s; }

	void update(uint8_t h, uint8_t m, uint8_t s) {
		_h = h; _m = m; _s = s;
		ready();
	}

protected:
	void _enable(bool);
	unsigned _sleepmode();

private:
	volatile uint8_t _h, _m, _s;
};

#endif
