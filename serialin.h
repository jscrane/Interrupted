#ifndef __SERIALIN_H__
#define __SERIALIN_H__

class SerialIn: public SerialDevice {
public:
	SerialIn(unsigned long baud, unsigned id): 
		SerialDevice(baud, id), _head(0), _n(0) {}

	void begin();
	void enable(bool enable = true);

	// returns the next character available for input or -1
	int read();

	// whether data is available to read
	bool available() { return _n > 0; }

	void on_input(byte b);

private:
	volatile byte _rx_buf[16];
	volatile byte _head, _n;
};

#endif
