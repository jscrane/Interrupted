#ifndef __SERIALIN_H__
#define __SERIALIN_H__

class SerialIn: public SerialDevice {
public:
	SerialIn(unsigned id, unsigned long baud = 0): 
		SerialDevice(id, baud), _head(0), _n(0) {}

	// enabled by default
	bool begin();

	// returns the next character available for input or -1
	int read() {
		noInterrupts();
		if (_n == 0)
			return -1;
		byte b = _rx_buf[_head++];
		_n--;
		if (_head == sizeof(_rx_buf))
			_head = 0;
		interrupts();
		return b;
	}

	// whether data is available to read
	bool available() { return _n > 0; }

	void on_input(byte b) {
		if (_n < sizeof(_rx_buf)) {
			byte tail = (_head + _n) % sizeof(_rx_buf);
			_rx_buf[tail] = b;
			_n++;
			ready();
		}
	}

protected:
	void _enable(bool);

private:
	volatile byte _rx_buf[16];
	volatile byte _head, _n;
};

#endif
