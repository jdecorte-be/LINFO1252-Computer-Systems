#include "tatas.h"

volatile bool locker = UNLOCKED;

bool test_and_set(volatile bool *ptr, int val)
{
    bool old = *ptr;
    __asm__ __volatile__ (
        "lock xchg %0, %1"
        : "+r" (val), "+m" (*ptr)
        :
        : "memory"
    );
    return old;
}

void lock()
{
    do
    {
        while(locker == LOCKED);
    }   while (test_and_set(&locker, LOCKED));
}

void unlock()
{
    test_and_set(&locker, UNLOCKED);
}

