// Serial output for the ATTiny84
// This code is derived from the SerialOut class from the atmega328p
// implementation in the Interrupted library and ideas from 
// http://electronut.in/serial-communications-with-the-attiny84/

#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "device.h"
#include "serial.h"
#include "serialout.h"

#define TRANSMISSION_SIZE 8  // not including 1 start bit + 1 stop bit
// #define TRANSMISSION_SIZE 10  // Add 2 idle bits but not including 1 start bit + 1 stop bit

volatile unsigned char current_byte = 0;
volatile int next_bit_index = -1;  // Index of next bit to send; -1 = not sending data yet

static SerialOut *device;

void send_bit_using_pin(unsigned int pinTX)
{
    // serial data sent least significant bit first
    // start(low)-0-1-2-3-4-5-6-7-stop(high)
    // 10 bits sent per packet

    if (next_bit_index < 0)
    {
        // start bit is low
        bitClear(PORTA, pinTX);

        // set bit index for first bit
        next_bit_index = 0;
    }
    else if (next_bit_index >= TRANSMISSION_SIZE)
    {
        // idle or stop bit is high
        bitSet(PORTA, pinTX);
        next_bit_index = -1;
    }
    #if TRANSMISSION_SIZE > 8
    else if (next_bit_index >= 8)
    {
        // Tranmission frame has idle bits, idle and stop bits are high
        bitSet(PORTA, pinTX);
        next_bit_index++;
    }
    #endif
    else
    {
        // data bits:
        // next_bit_index is in [0, 7]
        // extract relevant bit from data
        bitWrite(PORTA, pinTX, bitRead(current_byte, next_bit_index));
        next_bit_index++;
    }

}

bool SerialOut::begin() {
	device = this;
    _awake = false;

    _next_char_index = -1;

    // use the id as the TX pin, set the idle line state to high
    digitalWrite(this->id(), HIGH);
    pinMode(this->id(), OUTPUT);

	return false;
}

void SerialOut::_enable(bool e) {
    bitSet(TIFR0, OCF0A);   // Ensure the interrupt flag is clear to start
    bitWrite(TIMSK0, OCIE0A, e);
}

void SerialOut::start(char const *ptr, int buffer_offset)
{
    if (_next_char_index < 0)
    {
        // Not currently transmitting, so start one
        _next_char_index = buffer_offset;
        wake();
        enable();
        write(ptr);
    }
}

// Prepare for transmission of characters from the given buffer
void SerialOut::write(char const *ptr) {
	if (is_enabled())
    {
		uint8_t saved_status = SREG;
		cli();
        _tx_ptr = ptr;
        bitSet(TIFR0, OCF0A);   // Ensure the interrupt flag is clear to start

        TCNT0 = 0;      // Reset the timer
        bitSet(TCCR0B, CS02);  // Set the clock prescale bit for divide by 256 to start the clock

        next_bit_index = -1;    // Indicate that a new character is to be started
        do_output();
		SREG = saved_status;
	}
}

const char SerialOut::more()
{
    return _tx_ptr && _next_char_index >= 0 && _tx_ptr[_next_char_index];
}

char SerialOut::next()
{
    char next_char = _tx_ptr[_next_char_index];
    this->advance_to_next_character();
    return next_char;
}

void SerialOut::advance_to_next_character()
{
    _next_char_index++;
}

void SerialOut::finished()
{
    _tx_ptr = 0;
    _next_char_index = -1;
}

// Output a single bit of a transmission frame; may called from interrupt service routine
// If no more characters, then signal that the device is ready
void SerialOut::do_output()
{
    if (next_bit_index >= 0)
    {
        // Handle the transmission of the next bit
        send_bit_using_pin(this->id());
    }
    else
    {
        // Check to see if the current null terminated string of bytes has been sent
    	if (more())
        {
            current_byte = next();
            send_bit_using_pin(this->id());
    	}
        else
        {
            finished();
            bitClear(TCCR0B, CS02);  // stop the clock for the timer
    		ready();
    	}
    }
}

// Output the next bit of the current byte
ISR(TIM0_COMPA_vect)
{
	if (device)
		device->do_output();
}
