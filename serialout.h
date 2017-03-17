#ifndef __SERIALOUT_H__
#define __SERIALOUT_H__

#include "serial.h"

/**
 * Simple Serial output device
 */
class SerialOut: public SerialDevice {
public:
	SerialOut(unsigned id, unsigned long baud = 0):
		SerialDevice(id, baud), _tx_ptr(0), _next_char_index(-1) {}

	// not enabled by default
	bool begin();

	// writes a string

	bool write(char const *ptr);
	void do_output();

	// Return true when there are still bytes to output
	bool transmitting() {return _tx_ptr != 0; }
	virtual void start(char const *ptr, int offset=0);
	virtual const char more();
	virtual char next();
	virtual void advance_to_next_character();
	virtual void finished();


protected:
	void _enable(bool e);
	volatile char const *_tx_ptr;
	volatile int  _next_char_index;	 // Index of next byte to output

private:
};

#endif
