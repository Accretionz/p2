#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	/* TODO Phase 3 */
	size_t count; // Number of resources
	queue_t waiting_threads;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t sem = malloc(sizeof(struct semaphore));
	if (!sem) return NULL;

	sem->count = count;
	sem->waiting_threads = queue_create();
	if (!sem->waiting_threads)
	{
		free(sem);
		return NULL;
	}

	return sem;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	if (!sem || queue_length(sem->waiting_threads) > 0)
	{
		return -1;
	}

	queue_destroy(sem->waiting_threads);
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	if (!sem) return -1;

	while(sem->count = 0)
	{
		uthread_block();
		queue_enqueue(sem->waiting_threads, (void *)pthread_self());
	}

	sem->count--;

}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
	if (!sem) return -1;

	sem->count++;

	if (queue_length(sem->waiting_threads) > 0)
	{
		uthread_t tid;
		queue_dequeue(sem->waiting_threads, (void **)&tid);
		uthread_unblock(tid);
	}

	return 0;
}

