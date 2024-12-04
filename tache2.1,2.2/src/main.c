#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdatomic.h>

#define LOCKED 1
#define UNLOCKED 0
volatile bool *lock = 0;
int nThread = 0;

bool test_and_set(volatile bool *ptr, int val)
{
    __asm__ __volatile__ ( "xchg %0,%1" : "+r"(val), "+m"(*ptr) : : "memory" );
}

void lock()
{
    while (test_and_set(lock, 1)) {}
}

void unlock()
{
    test_and_set(lock, 0);
}




// 2.2 --------------------------------------------

void *thread_function(void *arg)
{
    (void)arg;
    for (int i = 0; i < 32768 / nThread; i++)
    {
        lock();
        for (int i=0; i<10000; i++);
        unlock();
    }
    return NULL;
}

int main(int ac, char **av)
{

    if(ac != 2)
    {
        printf("Usage: %s <number_of_threads>\n\n", av[0]);
        return (1);
    }

    nThread = atoi(av[1]);
    if(nThread < 1)
    {
        printf("Error: number of threads must be at least 1\n");
        return (1);
    }

    pthread_t thread[nThread];
    for (int i = 0; i < nThread; i++)
    {
        pthread_create(&thread[i], NULL, thread_function, NULL);
    }

    for (int i = 0; i < nThread; i++)
    {
        pthread_join(thread[i], NULL);
    }

    return (0);
}