#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/**
 * A Watchdog timer. Its resolution is only 1s right now.
 */
class Watchdog: public Timer {
public:
	Watchdog(int id, unsigned secs): Timer(id, secs) {}

	void begin();
	void enable(bool enable = true);
};

#endif
