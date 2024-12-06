#include "tatas.h"
// 2.4 --------------------------------------------



void sem_init_tas(t_sem_tatas *sem, int value)
{
	sem->value = value;
	sem->mutex = malloc(sizeof(int));
    if (sem->mutex == NULL)
    {
        perror("malloc failed");
        exit(1);
    }
	*sem->mutex = UNLOCKED;
}

void sem_destroy_tas(t_sem_tatas *sem)
{
	free(sem->mutex);
}

int sem_wait_tas(t_sem_tatas *sem)
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
	return 0;
}

void sem_post_tas(t_sem_tatas *sem)
{
	lock(sem->mutex);
	sem->value++;
	unlock(sem->mutex);
}
