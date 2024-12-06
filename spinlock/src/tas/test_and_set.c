#include "tas.h"

volatile int locker = UNLOCKED;

int test_and_set(volatile int *ptr, int val)
{
	__asm volatile("lock xchg %0,%1"
		     : "=r" (val), "+m" (*ptr)
		     : "0" (val)
		     : "memory");
    return val;
}

void lock()
{
    while (test_and_set(&locker, LOCKED) == LOCKED);
}

void unlock()
{
    test_and_set(&locker, UNLOCKED);
}