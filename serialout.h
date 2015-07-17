#ifndef __SERIALOUT_H__
#define __SERIALOUT_H__

/**
 * Simple Serial output device
 */
class SerialOut: public SerialDevice {
public:
	SerialOut(unsigned id, unsigned long baud = 0): 
		SerialDevice(id, baud), _tx_ptr(0) {}

	// not enabled by default
	bool begin();

	// writes a string
	void write(char const *ptr);
	void do_output();

protected:
	void _enable(bool) {}

private:
	volatile char const *_tx_ptr;
};

#endif
