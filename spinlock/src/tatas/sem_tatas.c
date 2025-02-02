#include "tatas.h"
// 2.4 --------------------------------------------



int sem_init_tatas(t_sem_tatas *sem, int value)
{
	sem->value = value;
	sem->mutex = malloc(sizeof(int));
    if (sem->mutex == NULL)
    {
        perror("malloc failed");
        return (EXIT_FAILURE);
    }
	*sem->mutex = UNLOCKED;
	return (EXIT_SUCCESS);
}

void sem_destroy_tatas(t_sem_tatas *sem)
{
	free(sem->mutex);
}

int sem_wait_tatas(t_sem_tatas *sem)
{
	lock(sem->mutex);
	int tmp = sem->value;
	unlock(sem->mutex);
	while (tmp == 0)
	{
		lock(sem->mutex);
		tmp = sem->value;
		unlock(sem->mutex);
	}
	lock(sem->mutex);
	sem->value--;
	unlock(sem->mutex);
	return (EXIT_SUCCESS);
}

void sem_post_tatas(t_sem_tatas *sem)
{
	lock(sem->mutex);
	sem->value++;
	unlock(sem->mutex);
}
