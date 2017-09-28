#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
#define ARCH atmega328p
#else
#error "Unknown arch!"
#endif
#define PROC_ARCH(f) <../ARCH/f>

