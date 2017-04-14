#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "serial.h"

void SerialDevice::init() {
	if (_baud) {
		unsigned prescale = ((F_CPU) / 16 + (_baud / 2)) / _baud - 1;

		uint8_t saved_SREG = SREG;	// Save the interrupt flag
		cli();				// disable interrupts while initializing the USART
		UBRR0H = (prescale >> 8) & 0xff;
		UBRR0L = prescale & 0xff;
		UCSR0C = bit(UCSZ00) | bit(UCSZ01);  // 8 bits, no parity, 1 stop bit
		SREG = saved_SREG;	// restore the interrupt flag
	}
}

unsigned SerialDevice::_sleepmode() {
	return SLEEP_MODE_IDLE;
}

// call to turn off device altogether
void SerialDevice::sleep()
{
	power_usart0_disable(); // Power down the USART
}

// call to turn device on again
void SerialDevice::wake()
{
	power_usart0_enable();	// Ensure USART powered up
	this->init(); 			// After power on, the module must be initialized
}
