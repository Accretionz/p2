# User-level thread library

## Summary

This program, `User-level thread library`, is the implementation of a user-level
thread library for linux. One of the main goals is to be able to understand how
multiple threads can run in the same context, in addition to writing our own
test cases. The library is able to create new threads, schedule
their execution round-robin style, and have preemption and semaphores. 

## Implementation

The implementation of this program follows four distinct steps:

1. Making the queue API
2. Making the uthread API
3. Making the semaphore API
4. Implmenting preemption

### Makefile

An advanced Makefile is required for this project. The one provided in the class
discussion was refereced, with a few parts edited in order to generate the
appropriate libraries, namely the arflags. Other than that, the makefile mostly
followed the same format, with keeping everything as general as possible so it
can easily be applied to any files we wished to add without having to add too
many new lines of code.

### Queue API

For the Queue API, we first created structs for the node and queue, since we
decided to use a linked list for the data structure. This way, it's easy to
traverse the list, and it can even traverse backwards thanks to the tail, which
is needed for the *delete()* function. In addition, unlike the slower array,
linked lists can append in *O(1)* time. The Queue API includes functions that
allow for *enqueue*, *dequeue* as the primary method of altering the queue.
This is done by utilizing the head and tails of the queue to keep track of the 
nodes within the queue system, which then allows for proper functionality of the
system created.

### uthread API

Phase 2, the uthread API, was the most challenging part of the project. An enum
was used to keep track of the states. At first, we used macros, but they were
causing unknown errors, which is why we switched to using enums. Our main data
structure is the Thread Control Block (TCB). At first, it was pretty simple, but
as we continued implementing, we added more features like state, context, and
func as we realized we'd need them. We used global variables for the ready and
zombie queue, to allow for seamless switching and storing of the queues, and
current_thread and next_thread were also kept global to be accessed by any
function. 

#### uthread_yield()

For uthread_yield(), our goal was to be able to change a thread's state to
**READY**, then place it into the back of the queue using *enqueue*. Then, the
next thread has to be dequeued to be changed to **RUNNING**, since we want to
check for every thread in our queue. A context switch is used at the end to save
the current context and activate the next. 

#### uthread_exit()

For uthread_exit() it creates a zombie thread from the current thread and 
changes the state to zombie. Which then gets enqueued to the zombie_queue that 
will later be freed in the uthread_run function.

#### uthread_create()

For uthread_create(), we dynamically allocate space for new_thread and
initialize it with a state and stack. We also dynamically allocate space for the
context of new_thread while also having error management checks when it is 
unable to create the space for it.

#### uthread_run()

For uthread_run(), we created a ready and zombie queue before initializing the
idleThread that is used to save the point in which we called the function
uthread_run as it needs to exit back to where it entered from. A while loop is
used to constantly yield when there is threads in the ready_queue while also
checking for the zombie_queue and destroying the stack before freeing it.

#### uthread_block() & uthread_unblock()

For the blocking and unblocking of uthread which is used in semaphores, we
either changed the state to block and yield or checked if the thread exist and
the state of it before changing it back to a ready state.

### Semaphore API

The implementation of semaphore is not fully correct as it only passes the
buffer test case and some parts of the other test cases. This maybe due to our
implementation of the sem_up and sem_down functions.

#### sem_create()

For sem_create, we dynamically allocate space for sem and initialize the count.
Creating a queue for the waiting threads and freeing it if it does not exist.

#### sem_destroy()

For sem_destory, we checked if the semaphore is NULL or there is still processes
within the waiting thread, if not then we destroyed the queue for waiting thread
then free sem.

#### sem_down()

For sem_down, we checked if count of sem is 0, if so then we enqueue current
thread to waiting threads and block it before decrementing the count

#### sem_up()

For sem_up, we increment count, then check for length of waiting threads, before
dequeueing the process from the waiting threads queue to unblock it.

### Preemption

For preemption, we were mostly confused on the implementation of it. For
preempt_disable we just blocked the SIGVTALRM signal and preempt_enable 
unblocked it. For preempt_start, we ignored the signal at the start until
preempt is called to set it up and preempt stop was used to reset the timer.

### Test cases

We included more test cases for queue_tester_example.c that test for multiple
enqueue and dequeue sequences, length of queue, enqueue null, dequeue empty, 
and destroying a non empty queue. We also implemented a tester for preempt, but
it stalls forever due to the infinite while loop we implemented within the 
thread in an attempt to have it yield to the next thread.

### Additional notes

Sources used were videos online and lecture notes regarding threading, 
semaphores and preempts. Also used source codes provided by the professor and 
had assistance from the tutor for ECS150.
