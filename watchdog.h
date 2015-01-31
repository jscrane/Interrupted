#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/**
 * A Watchdog timer. Its resolution is only 1s right now.
 */
class Watchdog: public Timer {
public:
	Watchdog(unsigned secs, unsigned id): Timer(secs, id) {}

	void begin();
	void enable(bool enable = true);
};

#endif
