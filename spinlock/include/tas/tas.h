#ifndef TAS_H
#define TAS_H

#define LOCKED 1
#define UNLOCKED 0

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

typedef struct s_sem_tas
{
	volatile int value;
	int *mutex;
} t_sem_tas;

void lock(volatile int *ptr);
void unlock(volatile int *ptr);

#endif