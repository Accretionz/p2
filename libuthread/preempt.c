#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

static struct itimerval preemption_timer;
static struct sigaction preemption_handler;

void preempt_disable(void)
{
	/* TODO Phase 4 */
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
}

void preempt_start(bool preempt)
{
	/* TODO Phase 4 */
	preemption_handler.sa_handler = SIG_IGN; // Ignore the signal initially
    preemption_handler.sa_flags = 0;
    sigemptyset(&preemption_handler.sa_mask);
    sigaction(SIGVTALRM, &preemption_handler, NULL);

    if (preempt) {
        preemption_handler.sa_handler = SIG_DFL; // Restore the default handler
        sigaction(SIGVTALRM, &preemption_handler, NULL);

        preemption_timer.it_value.tv_sec = 0;
        preemption_timer.it_value.tv_usec = 1000000 / HZ;
        preemption_timer.it_interval = preemption_timer.it_value;
        setitimer(ITIMER_VIRTUAL, &preemption_timer, NULL);
    }
}

void preempt_stop(void)
{
	/* TODO Phase 4 */
	preemption_timer.it_value.tv_sec = 0;
    preemption_timer.it_value.tv_usec = 0;
    preemption_timer.it_interval = preemption_timer.it_value;
    setitimer(ITIMER_VIRTUAL, &preemption_timer, NULL);
}

