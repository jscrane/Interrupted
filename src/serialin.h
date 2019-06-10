#ifndef __SERIALIN_H__
#define __SERIALIN_H__

class SerialIn_: public SerialDevice {
public:
	// enabled by default
	bool begin();

	// called from interrupt handler
	virtual void on_input(uint8_t b) = 0;

protected:
	void _enable(bool);

	SerialIn_(unsigned id, unsigned long baud):
		SerialDevice(id, baud) {}
};

template<unsigned BUF=16>
class SerialIn: public SerialIn_ {
public:
	SerialIn(unsigned id, unsigned long baud = 0):
		SerialIn_(id, baud), _head(0), _n(0) {}

	// returns the next character available for input or -1
	int read() {
		Atomic block;
		if (_n == 0)
			return -1;
		uint8_t b = _rx_buf[_head++];
		_n--;
		if (_head == BUF)
			_head = 0;
		return b;
	}

	// whether data is available to read
	bool available() { return _n > 0; }

	void on_input(uint8_t b) {
		if (_n < BUF) {
			uint8_t tail = (_head + _n) % sizeof(_rx_buf);
			_rx_buf[tail] = b;
			_n++;
			ready();
		}
	}

private:
	volatile uint8_t _rx_buf[BUF];
	volatile uint8_t _head, _n;
};

#endif
