#include <avr/sleep.h>
#include "device.h"
#include "busy.h"

unsigned Busy::sleepmode() {
	return SLEEP_MODE_IDLE;
}
