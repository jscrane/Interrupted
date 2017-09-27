#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
#include "../atmega328p/device.cpp"
#else
#error "Unknown arch!"
#endif
