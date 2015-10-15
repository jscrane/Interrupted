#include <Energia.h>

#include "device.h"
#include "delay.h"

unsigned Delay::_sleepmode() {
	return LPM1_bits;
}
