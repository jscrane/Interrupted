#include <Arduino.h>
#include <avr/sleep.h>

#include "device.h"
#include "delay.h"

unsigned Delay::_sleepmode() {
	return SLEEP_MODE_IDLE;
}
