#include "tas.h"


// 2.2 --------------------------------------------

int nThread = 0;

void *thread_function(void *arg)
{
    (void)arg;
    for (int j = 0; j < 32768 / nThread; j++)
    {
        lock();
        for (int i=0; i< 10000; i++);
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
        pthread_create(&thread[i], NULL, thread_function, NULL);

    for (int i = 0; i < nThread; i++)
        pthread_join(thread[i], NULL);

    return (0);
}