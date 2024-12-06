#include "tas.h"

int test_and_set(volatile int *ptr, int val)
{
	__asm volatile("lock xchg %0,%1"
		     : "=r" (val), "+m" (*ptr)
		     : "0" (val)
		     : "memory");
    return val;
}

void lock(volatile int *ptr)
{
	while(test_and_set(ptr, LOCKED));
}

void unlock(volatile int *ptr)
{
    test_and_set(ptr, UNLOCKED);
}

