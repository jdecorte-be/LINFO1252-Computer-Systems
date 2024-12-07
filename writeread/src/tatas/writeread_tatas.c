#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "tatas.h"

#define NREAD 2540
#define NWRITE 640

volatile int mut_read = UNLOCKED;
t_sem_tatas wrt;

int current_readers = 0;

int counter_readers = 0;
int counter_writers = 0;

void *writer()
{
    while (42)
    {
        sem_wait_tatas(&wrt);

        if (counter_writers >= NWRITE)
        {
            sem_post_tatas(&wrt);
            break;
        }
        counter_writers++;

        // section critique

        for (int i = 0; i < 10000; i++);

        sem_post_tatas(&wrt);
    }
    return NULL;
}

void *reader()
{
    while (42)
    {
        lock(&mut_read);

        ///// SECTION CRITIQUE
        if (counter_readers >= NREAD)
        {
            unlock(&mut_read);
            break;
        }
        current_readers++;
        counter_readers++;
        if (current_readers == 1)
            sem_wait_tatas(&wrt);
        unlock(&mut_read);

        for (int i = 0; i < 10000; i++);

        lock(&mut_read);
        current_readers--;

        if (current_readers == 0)
            sem_post_tatas(&wrt);
        /////////

        unlock(&mut_read);
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

    if (sem_init_tatas(&wrt, 1) != 0)
    {
        perror("sem_init_tatas");
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
    sem_destroy_tatas(&wrt);

    return 0;
}
