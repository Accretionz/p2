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

#define READY 0
#define RUNNING 1
#define BLOCKED 2
#define ZOMBIE 3

typedef enum {
	READY,
	RUNNING,
	BLOCKED,
	ZOMBIE,
} uthread_state;


struct uthread_tcb {
	/* TODO Phase 2 */
	uthread_t tid;
	uthread_state state;
	uthread_ctx_t *context;
	void* stack;
	void *arg;
	uthread_func_t func;
};

struct thread_system {
	queue_t thread_queue;
	queue_t current_thread;
}

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	return current_thread;
}

void uthread_yield(void) //call uthread_ctx_switch to make it so one thread thats currently running tells the actual operating system it wants to run another thread
//context has the info that defines the thread
{
	/* TODO Phase 2 */
	preempt_disable();
	struct uthread_tcb next;
	if(current_thread->state = RUNNING){
		current_thread->state = READY;
		queue_enqueue(current_thread, thread_queue); //putting the active thread in the back of the queue
	}

	uthread_tcb thread = NULL;
	queue_dequeue(current_thread, (void**)&thread);

	current_thread->state = RUNNING;

	uthread_ctx_thread(current_thread->context, next->context);
	preempt_enable();
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	current_thread->state = ZOMBIE;

	uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	struct uthread_tcb *new_thread = (struct uthread_tcb *)malloc(sizeof(struct uthread_tcb));
	if (new_thread == NULL)
	{
		return -1;
	}

	new_thread->tid = 1;
	new_thread->state = READY;
	new_thread->stack = uthread_ctx_alloc_stack();
	if (new_thread->stack == NULL)
	{
		free(new_thread);
		return -1;
	}

	if (uthread_ctx_init(new_thread->context, new_thread->stack, func, arg) == -1)
	{
		uthread_ctx_destroy_stack(new_thread->stack);
		free(new_thread);
		return -1;
	}

	queue_enqueue(thread_queue, new_thread);

	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg) //initializes all data structures to make data scheduling work, just a queue - if thread yields, it should take a new ready thread from the front of the queue and put prev running thread in the back - initializes in run
//makes an idle thread - a TCB information that holds the ocntext for the main
//process - when a program is run, the kernel makes context and stack for the
//process - going to have to leave uthread_run and cont in apps main func
//needs idle thread to store prrocess' context, idle thread also checks for
//zombies and frees them, and checks if there is anything left before it leaves
//will have idle thread and app thread
{
	/* TODO Phase 2 */
	// Idle thread creation
	void *idleStack = uthread_ctx_alloc_stack();
	if (idleStack == NULL)
	{
		return -1;
	}

	if (uthread_ctx_init(&idle_ctx, idleStack, NULL, NULL) == -1)
	{
		uthread_ctx_destroy_stack(idleStack);
		return -1;
	}

	// Creating thread
	if (uthread_create(func, arg) == -1)
	{
		uthread_ctx_destroy_stack(idleStack);
		return -1;
	}

	if (preempt)
	{
		preempt_enable();
	}
	else
	{
		preempt_disable();
	}

	while (queue_length(thread_queue) > 0)
	{	
		// Dequeue thread to be run next
		struct uthead_tcb *next_thread;
		if (queue_dequeue(thread_queue, (void**)&next_thread) == -1)
		{
			uthread_ctx_destroy_stack(idleStack);
			return -1;
		}

		// If no threads ready to run, switch to idle
		if (next_thread == NULL)
		{
			next_thread = &idle_ctx;
		}

		uthread_ctx_switch(&idle_ctx, next_thread->context); // Switching to next thread
	}

	uthread_ctx_destroy_stack(idleStack);
}

void uthread_block(void)
{
	/* TODO Phase 3 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
}

