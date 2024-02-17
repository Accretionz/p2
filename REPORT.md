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
can easily be applied to any fils we wished to add without having to add too
many new lines of code.

### Queue API

For the Queue API, we first created structs for the node and queue, since we
decided to use a linked list for the data structure. This way, it's easy to
traverse the list, and it can even traverse backwards thanks to the tail, which
is needed for the *delete()* function. In addition, unlike the slower array,
linked lists can append in *O(1)* time.

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

#### Thread_yield()

For thread_yield(), our goal was to be able to change a thread's state to
**READY**, then place it into the back of the queue using *enqueue*. Then, the
next thread has to be dequeued to be changed to **RUNNING**, since we want to
check for every thread in our queue. A context switch is used at the end to save
the current context and activate the next.  

### semaphore API

stuff

### preemption

stuff

### Test cases

### Additional notes

