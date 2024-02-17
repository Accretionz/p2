#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

#define TEST_ASSERT(assert)                \
do {                                    \
    printf("ASSERT: " #assert " ... ");    \
    if (assert) {                        \
        printf("PASS\n");                \
    } else    {                            \
        printf("FAIL\n");                \
        exit(1);                        \
    }                                    \
} while(0)

void thread2(void *arg){
  (void)arg;
  uthread_exit();
}

void thread1(void *arg){
  (void)arg;

  uthread_create(thread2, NULL);
  while(1){}
}


int main(){
  bool preempt = true;
  uthread_run(preempt, thread1, NULL);
}
