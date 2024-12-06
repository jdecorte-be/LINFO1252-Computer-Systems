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



void* writer(void *arg)
{   
    (void)arg;
    while(1)
    {   
        sem_wait(&wrt);

        if (counter_writers >= NWRITE){
            sem_post(&wrt);
            break;
        }
        counter_writers++;
        //printf("counter writers %d\n",counter_writers);

        // section critique
        
        //printf("writing.. \n");
        
        for (int i=0; i<10000; i++);
        
        sem_post(&wrt);
        
        
    }
    return NULL;
}

void* reader(void *arg)
{   
    (void)arg;
    while (1)
    {
        pthread_mutex_lock(&mut_read);
        // section critique
        //printf("nread = %d \n", counter_readers);
        if (counter_readers >= NREAD){
            pthread_mutex_unlock(&mut_read);
            break;
        }
        current_readers++;
        counter_readers++;
        if (current_readers==1)
        { // arrivée du premier reader
            sem_wait(&wrt);
        }
        pthread_mutex_unlock(&mut_read);



        
        //printf("reading.. \n");
        for (int i=0; i<10000; i++);
        
        pthread_mutex_lock(&mut_read);
        // section critique
        current_readers--;
        if(current_readers==0)
        { // départ du dernier reader
            sem_post(&wrt);
        }
        pthread_mutex_unlock(&mut_read);
    }
    return NULL;
}

int main(int ac, char **av)
{   
    if (ac != 3)
    {
        printf("Usage: %d <number_of_readers_writers>\n\n", ac);
        return (1);
    }

    int nReaders = atoi(av[1]);
    if (nReaders < 1)
    {
        printf("Error: number of readers must be at least 1 \n");
        return (1);
    }

    int nWriters = atoi(av[2]);
    if (nWriters < 1)
    {
        printf("Error: number of writers must be at least 1 \n");
        return (1);
    }

    int err;

    err = sem_init(&wrt, 0,1);
    if(err!=0) {
        printf("sem_init %d \n", err);
    }

    err = pthread_mutex_init(&mut_read, NULL);
    if(err!=0) {
        printf("mutex_init %d \n", err);
    }


    pthread_t readers[nReaders];
    for (int i = 0 ; i < nReaders ; i++){
        pthread_create(&readers[i], NULL, reader, NULL);
    }

    pthread_t writers[nWriters];
    for (int i = 0 ; i < nWriters ; i++){
        pthread_create(&writers[i], NULL, writer, NULL);
    }


    for (int i = 0 ; i < nReaders ; i++){
        pthread_join(readers[i], NULL);
    }

    for (int i = 0 ; i < nWriters ; i++){
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&wrt);
    pthread_mutex_destroy(&mut_read);

    return 0;
}



