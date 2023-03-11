#include <Arduino.h>

#include "device.h"
#include "delay.h"

unsigned Delay::_sleepmode() {
	return LPM1_bits;
}

bool Delay::begin() {
	return false;
}
