#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NCYCLE 1000000

typedef struct s_philosopher
{
    int id;
    pthread_t thread_id;
    pthread_mutex_t fork[2]; // 0 = left, 1 = right
}   t_philosopher;


void *run_philosopher(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;

    for (size_t i = 0; i < NCYCLE; i++)
    {
        //printf("Philosopher %d is thinking\n", philo->id);

        // avoid dead lock
        if (philo->id % 2 == 0)
        {
            pthread_mutex_lock(&philo->fork[0]);
            pthread_mutex_lock(&philo->fork[1]);
        }
        else
        {
            pthread_mutex_lock(&philo->fork[1]);
            pthread_mutex_lock(&philo->fork[0]);
        }

        //printf("Philosopher %d is eating\n", philo->id);

        pthread_mutex_unlock(&philo->fork[1]);
        pthread_mutex_unlock(&philo->fork[0]);
    }

    return (NULL);
}



int main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("Usage: %s <number_of_philosophers>\n\n", av[0]);
        return (1);
    }

    int nPhilo = atoi(av[1]);
    if (nPhilo < 2)
    {
        printf("Error: number of philosophers must be at least 2\n");
        return (1);
    }

    t_philosopher *philos = malloc(sizeof(t_philosopher) * nPhilo);
    if (!philos)
    {
        printf("Error: malloc failed\n");
        return (1);
    }

    for (int i = 0; i < nPhilo; i++)
    {
        philos[i].fork[0] = philos[i].fork[1] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    }

    for (int i = 0; i < nPhilo; i++)
    {
        philos[i].id = i;
        pthread_create(&philos[i].thread_id, NULL, run_philosopher, &philos[i]);
    }

    for (int i = 0; i < nPhilo; i++)
    {
        pthread_join(philos[i].thread_id, NULL);
    }

    // Clean Up
    for (int i = 0; i < nPhilo; i++)
    {
        pthread_mutex_destroy(&philos[i].fork[0]);
        pthread_mutex_destroy(&philos[i].fork[1]);
    }
    free(philos);
}