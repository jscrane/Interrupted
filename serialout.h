#ifndef __SERIAL_OUT_H__
#define __SERIAL_OUT_H__

/**
 * Simple Serial output device
 */
class SerialOut: public SerialDevice {
public:
	SerialOut(unsigned long baud, unsigned id): 
		SerialDevice(baud, id), _tx_ptr(0) {}

	void begin();

	// writes a string
	void write(char const *ptr);
	void do_output();

private:
	volatile char const *_tx_ptr;
};

#endif
