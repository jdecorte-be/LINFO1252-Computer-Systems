#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define NREAD 2540
#define NWRITE 640

pthread_mutex_t mut_read;
sem_t wrt;

int current_readers = 0;

int counter_readers = 0;
int counter_writers = 0;

void *writer()
{
    while (42)
    {
        sem_wait(&wrt);

        if (counter_writers >= NWRITE)
        {
            sem_post(&wrt);
            break;
        }
        counter_writers++;

        // section critique

        for (int i = 0; i < 10000; i++);

        sem_post(&wrt);
    }
    return NULL;
}

void *reader()
{
    while (42)
    {
        pthread_mutex_lock(&mut_read);

        ///// SECTION CRITIQUE
        if (counter_readers >= NREAD)
        {
            pthread_mutex_unlock(&mut_read);
            break;
        }
        current_readers++;
        counter_readers++;
        if (current_readers == 1)
            sem_wait(&wrt);
        pthread_mutex_unlock(&mut_read);

        for (int i = 0; i < 10000; i++);

        pthread_mutex_lock(&mut_read);
        current_readers--;

        if (current_readers == 0)
            sem_post(&wrt);
        /////////

        pthread_mutex_unlock(&mut_read);
    }
    return NULL;
}

int main(int ac, char **av)
{
    if (ac != 3)
    {
        printf("Usage: %s <number_of_readers> <number_of_writers>\n", av[0]);
        return (EXIT_FAILURE);
    }

    int nReaders = atoi(av[1]);
    int nWriters = atoi(av[2]);
    if (nReaders < 1 || nWriters < 1)
    {
        printf("Error: number of readers and writers must be at least 1\n");
        return (EXIT_FAILURE);
    }

    if (sem_init(&wrt, 0, 1) != 0)
    {
        perror("sem_init");
        return (EXIT_FAILURE);
    }

    if (pthread_mutex_init(&mut_read, NULL) != 0)
    {
        perror("pthread_mutex_init");
        return (EXIT_FAILURE);
    }

    pthread_t readers[nReaders];
    for (int i = 0; i < nReaders; i++)
    {
        if (pthread_create(&readers[i], NULL, reader, NULL) != 0)
        {
            perror("pthread_create");
            return (EXIT_FAILURE);
        }
    }

    pthread_t writers[nWriters];
    for (int i = 0; i < nWriters; i++)
    {
        if (pthread_create(&writers[i], NULL, writer, NULL) != 0)
        {
            perror("pthread_create");
            return (EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nReaders; i++)
    {
        if (pthread_join(readers[i], NULL) != 0)
        {
            perror("pthread_create");
            return (EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nWriters; i++)
    {
        if (pthread_join(writers[i], NULL) != 0)
        {
            perror("pthread_create");
            return (EXIT_FAILURE);
        }
    }

    // Clean up
    if (sem_destroy(&wrt) != 0)
    {
        perror("sem_destroy");
        return (EXIT_FAILURE);
    }
    if (pthread_mutex_destroy(&mut_read) != 0)
    {
        perror("pthread_mutex_destroy");
        return (EXIT_FAILURE);
    }

    return 0;
}
