#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NCYCLE 1000000
#define LEFT 0
#define RIGHT 1

typedef struct s_philosopher
{
    int id;
    pthread_t thread_id;
    pthread_mutex_t fork[2]; // 0 = left, 1 = right
} t_philosopher;

void *run_philosopher(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;

    for (size_t i = 0; i < NCYCLE; i++)
    {
        // avoid dead lock
        if (philo->id % 2 == 0)
        {
            pthread_mutex_lock(&philo->fork[LEFT]);
            pthread_mutex_lock(&philo->fork[RIGHT]);
        }
        else
        {
            pthread_mutex_lock(&philo->fork[RIGHT]);
            pthread_mutex_lock(&philo->fork[LEFT]);
        }

        pthread_mutex_unlock(&philo->fork[RIGHT]);
        pthread_mutex_unlock(&philo->fork[LEFT]);
    }

    return (NULL);
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("Usage: %s <number_of_philosophers>\n\n", av[0]);
        return (EXIT_FAILURE);
    }

    int nPhilo = atoi(av[1]);
    if (nPhilo < 2)
    {
        printf("Error: number of philosophers must be at least 2\n");
        return (EXIT_FAILURE);
    }

    t_philosopher *philos = malloc(sizeof(t_philosopher) * nPhilo);
    if (!philos)
    {
        perror("Error: malloc failed");
        return (EXIT_FAILURE);
    }

    for (int i = 0; i < nPhilo; i++)
    {
        if (pthread_mutex_init(&philos[i].fork[LEFT], NULL) != 0)
        {
            perror("Error initializing mutex");
            free(philos);
            return (EXIT_FAILURE);
        }
        if (pthread_mutex_init(&philos[i].fork[RIGHT], NULL) != 0)
        {
            perror("Error initializing mutex");
            free(philos);
            return (EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nPhilo; i++)
    {
        philos[i].id = i;
        if (pthread_create(&philos[i].thread_id, NULL, run_philosopher, &philos[i]) != 0)
        {
            perror("Error creating thread");
            free(philos);
            return (EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nPhilo; i++)
    {
        if (pthread_join(philos[i].thread_id, NULL))
        {
            perror("Error joining thread");
            free(philos);
            return (EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nPhilo; i++)
    {
        if (pthread_mutex_destroy(&philos[i].fork[LEFT]) != 0)
        {
            perror("Error destroying mutex");
            free(philos);
            return (EXIT_FAILURE);
        }

        if (pthread_mutex_destroy(&philos[i].fork[RIGHT]) != 0)
        {
            perror("Error destroying mutex");
            free(philos);
            return (EXIT_FAILURE);
        }
    }

    free(philos);
}