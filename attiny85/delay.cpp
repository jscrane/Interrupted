#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include "device.h"
#include "delay.h"

unsigned Delay::_sleepmode() {
	return SLEEP_MODE_IDLE;
}

bool Delay::begin() {
	power_timer0_enable();
	return false;
}
