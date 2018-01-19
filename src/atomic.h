#ifndef __ATOMIC_H__
#define __ATOMIC_H__

/**
 * Interrupts can occur at any time, unless explicitly disabled.
 * (Otherwise other useful Arduino features, such as the millisecond
 * timer, and the Serial class would not work.)
 *
 * Use this class to disable interrupts within a well-defined scope.
 */
class Atomic {
public:
	Atomic() { noInterrupts(); }
	~Atomic() { interrupts(); }
};

#endif
