#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	/* TODO Phase 3 */
	size_t count; // Number of resources
	queue_t waiting_threads; // Queue for threads waiting for resources
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	// Allocation of space for semaphore
	sem_t sem = malloc(sizeof(struct semaphore));
	if (!sem) return NULL;

	// Initialize sem count and waiting thread queue
	sem->count = count;
	sem->waiting_threads = queue_create();
	if (!sem->waiting_threads)
	{
		free(sem); // Free sem if queue can't be created
		return NULL;
	}

	return sem;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	// Check if semaphore is NULL or there is still waiting threads
	if (!sem || queue_length(sem->waiting_threads) > 0)
	{
		return -1;
	}

	// Destroy waiting threads queue and free semaphore
	queue_destroy(sem->waiting_threads);
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	if (!sem) return -1;

	// Disable preempts to prevent race conditions
	preempt_disable();

	while(sem->count == 0)
	{	
		// Block thread if no resource available
		queue_enqueue(sem->waiting_threads, (void*)uthread_current());
		uthread_block();
	}

	sem->count--;
	preempt_enable();
	
	return 0;

}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
	if (!sem) return -1;

	preempt_disable();

	sem->count++; // Increase semaphore count

	if (queue_length(sem->waiting_threads) > 0)
	{	
		// Unblocking first thread in queue
		struct uthread_tcb* tid;
		queue_dequeue(sem->waiting_threads, (void **)&tid);
		uthread_unblock(tid);
		uthread_yield();
	}

	preempt_enable();
	return 0;
}

