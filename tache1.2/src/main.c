#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define CAPACITY 8

pthread_mutex_t mutex_buffer = PTHREAD_MUTEX_INITIALIZER;
int buffer[CAPACITY] = {0};

sem_t sem_full;
sem_t sem_empty;

int size = 0;

void *consumer(void *arg)
{
    (void)arg;
    for (int i = 0; i < 131072; i++)
    {
        for (int j = 0; j < 10000; j++)
            ;

        int y;

        sem_wait(&sem_full);
        pthread_mutex_lock(&mutex_buffer);
        
        if(size > 0)
        {
            y = buffer[size - 1];
            size--;
        }    

        pthread_mutex_unlock(&mutex_buffer);
        sem_post(&sem_empty);

        printf("Consumer consumed item %d\n", y);
    }
    return (NULL);
}

void *producer(void *arg)
{
    (void)arg;
    int id = 0;
    for (int i = 0; i < 131072; i++)
    {
        for (int j = 0; j < 10000; j++)
            ;

        sem_wait(&sem_empty);
        pthread_mutex_lock(&mutex_buffer);
        if(size < CAPACITY)
        {
            buffer[size] = id;
            id++;
            size++;
        } else
            printf("Buffer is full\n");
        pthread_mutex_unlock(&mutex_buffer);
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
        pthread_create(&cons[i], NULL, consumer, NULL);
    }

    pthread_t prod[nProd];
    for (int i = 0; i < nProd; i++)
    {
        pthread_create(&prod[i], NULL, producer, NULL);
    }

    for (int i = 0; i < nCons; i++)
    {
        pthread_join(cons[i], NULL);
        pthread_join(prod[i], NULL);
    }

    sem_destroy(&sem_full);
    sem_destroy(&sem_empty);
    pthread_mutex_destroy(&mutex_buffer);

    return (0);
}