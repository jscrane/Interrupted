#ifndef __BUSY_H__
#define __BUSY_H__

/*
 * A device which is always ready. For when there's no alternative to polling.
 * This device is not enabled by default.
 * Add it to Devices last or it will mask other events.
 */
class Busy: public Device {
public:
	Busy(): Device(-1) {}

	bool begin() { return false; }

	bool is_ready() { return is_enabled(); }

	unsigned sleepmode();

protected:
	void _enable(bool) {}
};

#endif
