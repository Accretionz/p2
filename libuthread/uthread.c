#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

typedef enum {
	READY,
	RUNNING,
	BLOCKED,
	ZOMBIE,
} uthread_state;


struct uthread_tcb {
	uthread_state state;
	uthread_ctx_t *context;
	void* stack;
	void *arg;
	uthread_func_t func;
	struct semaphore *semaphore;
};

struct uthread_tcb *current_thread;
queue_t ready_queue;
queue_t zombie_queue;

struct uthread_tcb *uthread_current(void)
{
	return current_thread;
}

void uthread_yield(void) //call uthread_ctx_switch to make it so one thread thats currently running tells the actual operating system it wants to run another thread
//context has the info that defines the thread
{
	struct uthread_tcb *next_thread;

	preempt_disable();

	if (current_thread->state == RUNNING)
	{
		current_thread->state = READY;
		
		queue_enqueue(ready_queue, current_thread); //putting the active thread in the back of the queue
	}

	if (queue_dequeue(ready_queue, (void**)&next_thread) == 0)
	{	
		struct uthread_tcb *yielding_thread = current_thread;
		current_thread = next_thread;
		current_thread->state = RUNNING;

		uthread_ctx_switch(yielding_thread->context, next_thread->context);
	}

	preempt_enable();
}

void uthread_exit(void)
{
	struct uthread_tcb *zombie_thread = current_thread;
	zombie_thread->state = ZOMBIE;
	queue_enqueue(zombie_queue, zombie_thread);
	uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *new_thread = (struct uthread_tcb *)malloc(sizeof(struct uthread_tcb));
	if (new_thread == NULL)
	{
		return -1;
	}

	new_thread->state = READY;
	new_thread->stack = uthread_ctx_alloc_stack();
	if (new_thread->stack == NULL)
	{
		free(new_thread);
		return -1;
	}

	new_thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));

	if (new_thread->context == NULL)
	{
		uthread_ctx_destroy_stack(new_thread->stack);
		free(new_thread);
		return -1;
	}

	new_thread->semaphore = NULL;
	
	if (uthread_ctx_init(new_thread->context, new_thread->stack, func, arg) == -1)
	{
		uthread_ctx_destroy_stack(new_thread->stack);
		free(new_thread->context);
		free(new_thread);
		return -1;
	}

	queue_enqueue(ready_queue, new_thread);

	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg) //initializes all data structures to make data scheduling work
{
	if (preempt)
	{
		preempt_enable();
	}
	else
	{
		preempt_disable();
	}

	ready_queue = queue_create();
	if (ready_queue == NULL)
	{
		return -1;
	}

	zombie_queue = queue_create();
	if (zombie_queue == NULL)
	{
		return -1;
	}
	
	uthread_create(NULL, NULL);
	struct uthread_tcb *idleThread;
	queue_dequeue(ready_queue, (void**)&idleThread);
	idleThread->state = RUNNING;
	current_thread = idleThread;

	// Creating thread
	if (uthread_create(func, arg) == -1)
	{
		return -1;
	}

	while (queue_length(ready_queue) > 0)
	{	
		uthread_yield();
		while(queue_length(zombie_queue) > 0)
		{
			struct uthread_tcb *zombie_thread;
			if (queue_dequeue(zombie_queue, (void**)&zombie_thread) == 0)
			{
				uthread_ctx_destroy_stack(zombie_thread->stack);
				free(zombie_thread->context);
				free(zombie_thread);
			}
		}

	}

	queue_destroy(ready_queue);
	queue_destroy(zombie_queue);

	return 0;
}

void uthread_block(void)
{
	uthread_current()->state = BLOCKED;
	uthread_yield();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	if (uthread && uthread->state == BLOCKED)
	{
		uthread->state = READY;
	}
}

