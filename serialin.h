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
		cli();
		if (_n == 0)
			return -1;
		uint8_t b = _rx_buf[_head++];
		_n--;
		if (_head == sizeof(_rx_buf))
			_head = 0;
		sei();
		return b;
	}

	// whether data is available to read
	bool available() { return _n > 0; }

	void on_input(uint8_t b) {
		if (_n < sizeof(_rx_buf)) {
			uint8_t tail = (_head + _n) % sizeof(_rx_buf);
			_rx_buf[tail] = b;
			_n++;
			ready();
		}
	}

protected:
	void _enable(bool);

private:
	volatile uint8_t _rx_buf[16];
	volatile uint8_t _head, _n;
};

#endif
