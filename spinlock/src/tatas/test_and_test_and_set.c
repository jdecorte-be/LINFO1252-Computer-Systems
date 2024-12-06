#include "tatas.h"

volatile bool locker = UNLOCKED;

bool test_and_set(volatile bool *ptr, int val)
{
	__asm volatile("lock xchg %0,%1"
		     : "=r" (val), "+m" (*ptr)
		     : "0" (val)
		     : "memory");
    return val;
}

void lock()
{
    do
    {
        while (locker == LOCKED)
            ;
    } while (test_and_set(&locker, LOCKED));
}

void unlock()
{
    test_and_set(&locker, UNLOCKED);
}
