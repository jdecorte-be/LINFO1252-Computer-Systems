#include "tatas.h"
// 2.2 --------------------------------------------
#define NOPERATION 32768

volatile int mutex = UNLOCKED;

void *thread_function(void *arg)
{
    int n = *((int *)arg);
    for (int j = 0; j < NOPERATION / n; j++)
    {
        lock(&mutex);
        for (int i = 0; i < 10000; i++)
            ;
        unlock(&mutex);
    }
    return NULL;
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("Usage: %s <number_of_threads>\n\n", av[0]);
        return (EXIT_FAILURE);
    }

    int nThread = atoi(av[1]);
    if (nThread < 1)
    {
        printf("Error: number of threads must be at least 1\n");
        return (EXIT_FAILURE);

    }

    pthread_t thread[nThread];
    for (int i = 0; i < nThread; i++)
    {
        if (pthread_create(&thread[i], NULL, thread_function, &nThread) != 0)
        {
            perror("Error creating thread");
            return (EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nThread; i++)
    {
        if (pthread_join(thread[i], NULL) != 0)
        {
            perror("Error joining thread");
            return (EXIT_FAILURE);
        }
    }

    return (0);
}