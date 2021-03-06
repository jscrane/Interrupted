#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
#define ARCH atmega328p
#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__)
#define ARCH attiny85
#elif defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny44__)
#define ARCH attiny84
#elif defined(__MSP430FR5739__)
#define ARCH msp430fr5739
#elif defined(__MSP430G2553__)
#define ARCH msp430g2553
#else
#error "Unknown arch!"
#endif
#define PROC_ARCH(f) <../ARCH/f>

