#include "tas.h"
// 2.4 --------------------------------------------

int sem_init_tas(t_sem_tas *sem, int value)
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

void sem_destroy_tas(t_sem_tas *sem)
{
	free(sem->mutex);
}

int sem_wait_tas(t_sem_tas *sem)
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

void sem_post_tas(t_sem_tas *sem)
{
	lock(sem->mutex);
	sem->value++;
	unlock(sem->mutex);
}
