#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "serial.h"

#define IOCLK_PRESCALE 256

void SerialDevice::init() {
	if (_baud) {
		unsigned bit_timing_count = ((F_CPU) / IOCLK_PRESCALE + (_baud / 2)) / _baud - 1;

		uint8_t saved_SREG = SREG;	// Save the interrupt flag
		cli();	// disable interrupts while initializing the timer 0

		// Timer 0 is an 8 bit timer, and used for the timing of the serial port
		TCCR0A = 0;
		TCCR0B = 0;

		// Put Timer/Counter0 in CTC mode
		bitSet(TCCR0A, WGM01);

		// Count cycles for each bit
	    OCR0A = bit_timing_count;

		SREG = saved_SREG;	// restore the interrupt flag
	}
}

unsigned SerialDevice::_sleepmode() {
	return SLEEP_MODE_IDLE;
}

// call to turn off device altogether
void SerialDevice::sleep()
{
	if (true) //(_awake)
	{
		power_timer0_disable();    // Timer 0 is used to time the bits
		_awake = false;
	}
}

// call to turn device on again
void SerialDevice::wake()
{
	if (true) //(!_awake)
	{
		power_timer0_enable();	// Timer 0 is used to time the bits
		this->init(); 			// After power on, initialize the module
		_awake = true;
	}
}
