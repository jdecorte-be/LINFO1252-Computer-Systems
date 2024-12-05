#ifndef TAS_H
#define TAS_H

#define LOCKED 1
#define UNLOCKED 0

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

void lock();
void unlock();
bool test_and_set(volatile bool *ptr, int val);

#endif