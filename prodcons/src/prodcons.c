#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define CAPACITY 8
#define NITER 131072

int buffer[CAPACITY] = {0};

sem_t sem_full;
sem_t sem_empty;
pthread_mutex_t mutex_buffer = PTHREAD_MUTEX_INITIALIZER;

int in = 0;
int out = 0;

void *consumer(void *arg)
{
    int consumer_id = *((int *)arg);
    for (int i = 0; i < NITER; i++)
    {
        sem_wait(&sem_full);
        pthread_mutex_lock(&mutex_buffer);
        
        int y = buffer[out];
        out = (out + 1) % CAPACITY;

        printf("Consumer %d: %d\n", consumer_id, y);

        pthread_mutex_unlock(&mutex_buffer);
        for (int j = 0; j < 10000; j++);
        sem_post(&sem_empty);

    }
    return (NULL);
}

void *producer(void *arg)
{
    int id = *((int *)arg);
    for (int i = 0; i < NITER; i++)
    {
        sem_wait(&sem_empty);
        pthread_mutex_lock(&mutex_buffer);

        buffer[in] = id;
        in = (in + 1) % CAPACITY;

        printf("Produce %d\n", id);

        pthread_mutex_unlock(&mutex_buffer);
        for (int j = 0; j < 10000; j++);
        sem_post(&sem_full);
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
    sem_init(&sem_full, 0, 0);
    sem_init(&sem_empty, 0, CAPACITY);
    pthread_mutex_init(&mutex_buffer, NULL);

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

    sem_destroy(&sem_full);
    sem_destroy(&sem_empty);
    pthread_mutex_destroy(&mutex_buffer);

    return (0);
}