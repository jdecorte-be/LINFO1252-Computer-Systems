#include <stdio.h>
#include "tatas.h"

#define CAPACITY 8
#define NITER 131072

int buffer[CAPACITY] = {0};

t_sem_tatas sem_full;
t_sem_tatas sem_empty;
volatile int mutex_buffer = UNLOCKED;

int in = 0;
int out = 0;

int count_cons = 0;
int count_prod = 0;

void *consumer()
{
    while (42)
    {
        sem_wait_tatas(&sem_full);
        lock(&mutex_buffer);

        //// SECTION CRITIQUE
        if (count_cons >= NITER)
        {
            unlock(&mutex_buffer);
            sem_post_tatas(&sem_empty);
            break;
        }

        int y = buffer[out];
        out = (out + 1) % CAPACITY;
        count_cons++;

        printf("Consume %d\n", y);
        ////////////

        unlock(&mutex_buffer);
        sem_post_tatas(&sem_empty);
        for (int j = 0; j < 10000; j++)
            ;
    }
    return (NULL);
}

void *producer(void *arg)
{
    long id = (long)arg;
    while (42)
    {
        sem_wait_tatas(&sem_empty);
        lock(&mutex_buffer);

        //// SECTION CRITIQUE
        if (count_prod >= NITER)
        {
            unlock(&mutex_buffer);
            sem_post_tatas(&sem_full);
            break;
        }

        buffer[in] = id;
        in = (in + 1) % CAPACITY;
        count_prod++;

        printf("Produce %d\n", (int)id);
        ////////////

        unlock(&mutex_buffer);
        sem_post_tatas(&sem_full);
        for (int j = 0; j < 10000; j++)
            ;
    }
    return (NULL);
}

int main(int ac, char **av)
{
    if (ac != 3)
    {
        printf("Usage: %s <number_of_cons_threads> <number_of_prod_threads>\n\n", av[0]);
        return (EXIT_FAILURE);
    }

    int nCons = atoi(av[1]);
    int nProd = atoi(av[2]);

    if (nCons < 1 || nProd < 1)
    {
        printf("Error: number of consumers and producers must be at least 1\n");
        return (EXIT_FAILURE);
    }

    // Init semaphores
    if(sem_init_tatas(&sem_full, 0) != 0)
    {
        perror("sem_init");
        return (EXIT_FAILURE);
    }

    if(sem_init_tatas(&sem_empty, CAPACITY) != 0)
    {
        perror("sem_init");
        return (EXIT_FAILURE);
    }

    pthread_t cons[nCons];
    pthread_t prod[nProd];

    // Create threads
    for (int i = 0; i < nCons; i++)
    {
        if (pthread_create(&cons[i], NULL, consumer, NULL) != 0)
        {
            perror("pthread_create");
            return (EXIT_FAILURE);
        }
    }

    for (long i = 0; i < nProd; i++)
    {
        if (pthread_create(&prod[i], NULL, producer, (void *)i) != 0)
        {
            perror("pthread_create");
            return (EXIT_FAILURE);
        }
    }

    // Join all threads
    for (int i = 0; i < nCons; i++)
    {
        if (pthread_join(cons[i], NULL) != 0)
        {
            perror("pthread_join");
            return (EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nProd; i++)
    {
        if (pthread_join(prod[i], NULL) != 0)
        {
            perror("pthread_join");
            return (EXIT_FAILURE);
        }
    }

    // Cleanup
    sem_destroy_tatas(&sem_full);
    sem_destroy_tatas(&sem_empty);

    return (0);
}