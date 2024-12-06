#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "tas.h"

#define CAPACITY 8
#define NITER 131072

int buffer[CAPACITY] = {0};

t_sem_tas sem_full;
t_sem_tas sem_empty;
volatile int mutex_buffer = UNLOCKED;

int in = 0;
int out = 0;

void *consumer(void *arg)
{
    int consumer_id = *((int *)arg);
    for (int i = 0; i < NITER; i++)
    {
        sem_wait_tas(&sem_full);
        lock(&mutex_buffer);
        
        int y = buffer[out];
        out = (out + 1) % CAPACITY;

        printf("Consumer %d: %d\n", consumer_id, y);

        unlock(&mutex_buffer);
        for (int j = 0; j < 10000; j++);
        sem_post_tas(&sem_empty);

    }
    return (NULL);
}

void *producer(void *arg)
{
    int id = *((int *)arg);
    for (int i = 0; i < NITER; i++)
    {
        sem_wait_tas(&sem_empty);
        lock(&mutex_buffer);

        buffer[in] = id;
        in = (in + 1) % CAPACITY;

        printf("Produce %d\n", id);

        unlock(&mutex_buffer);
        for (int j = 0; j < 10000; j++);
        sem_post_tas(&sem_full);
    }
    return (NULL);
}

int main(int ac, char **av)
{
    if (ac != 3)
    {
        printf("Usage: %s <number_of_cons_threads> <number_of_prod_threads>\n\n", av[0]);
        return (1);
    }

    int nCons = atoi(av[1]);
    int nProd = atoi(av[2]);

    if (nCons < 1 || nProd < 1)
    {
        printf("Error: number of consumers and producers must be at least 1\n");
        return (1);
    }

    // ---
    sem_init_tas(&sem_full, 0);
    sem_init_tas(&sem_empty, CAPACITY);

    pthread_t cons[nCons];
    for (int i = 0; i < nCons; i++)
    {
        if(pthread_create(&cons[i], NULL, consumer, &i) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    pthread_t prod[nProd];
    for (int i = 0; i < nProd; i++)
    {
        if(pthread_create(&prod[i], NULL, producer, &i) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nCons; i++)
    {
        if(pthread_join(cons[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nProd; i++)
    {
        if(pthread_join(prod[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    return (0);
}