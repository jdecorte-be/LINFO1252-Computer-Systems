#ifndef TASTAS_H
#define TASTAS_H

#define LOCKED 1
#define UNLOCKED 0

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

typedef struct s_sem_tatas
{
	volatile int value;
	int *mutex;
} t_sem_tatas;

void lock(volatile int *ptr);
void unlock(volatile int *ptr);

#endif