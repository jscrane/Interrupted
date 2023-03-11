#include "proc_arch.h"
#include PROC_ARCH(device.cpp)

#include "atomic.h"

int Devices::select() {
        // so we don't miss an interrupt while checking...
        Atomic block;
        unsigned mode = SLEEP_MODE_NONE;
        for (int i = 0; i < _n; i++) {
                Device *d = _devices[i];
                if (d->is_ready())
                        return d->id();
                if (d->is_enabled())
                        mode = d->negotiate_mode(mode);
        }
        idle(mode);
        return -1;
}

void Devices::enable() {
	for (int i = 0; i < _n; i++)
		_devices[i]->enable(_devices[i]->begin());
}
