#ifndef __SERIALOUT_H__
#define __SERIALOUT_H__

class SerialOut_: public SerialDevice {
public:
	// not enabled by default
	bool begin();

	virtual bool next(uint8_t &b) = 0;

protected:
	SerialOut_(unsigned id, unsigned long baud): 
		SerialDevice(id, baud) {}

	void _enable(bool);
};

template<unsigned BUF=16>
class SerialOut: public SerialOut_, public Print {
public:
	SerialOut(unsigned id, unsigned long baud = 0): 
		SerialOut_(id, baud), _head(0), _tail(0), _empty(true), _full(false) {}

	// called from Print
	size_t write(uint8_t b) {
		Atomic block;
		if (_full)
			return 0;

		_buf[_tail++] = b;
		if (_tail == BUF) _tail = 0;

		_empty = false;
		_full = (_head == _tail);
		enable();
		return 1;
	}

	// called from interrupt handler
	bool next(uint8_t &b) {
		if (_empty) {
			ready();
			disable();
			return false;
		}
		
		b = _buf[_head++];
		if (_head == BUF) _head = 0;

		_full = false;
		_empty = (_head == _tail);
		return true;
	}
private:
	volatile uint8_t _buf[BUF];
	volatile uint8_t _head, _tail;
	volatile bool _empty, _full;
};

#endif
