#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

void test_queue_multiple(void)
{
	int data[] = {1, 2, 3, 4, 5}, *ptr;
	queue_t q;
	int i;

	fprintf(stderr, "*** TEST queue_multiple ***\n");

	q = queue_create();
	for (i = 0; i < 5; i++) {
		queue_enqueue(q, &data[i]);
	}

	for (i = 0; i < 5; i++) {
		queue_dequeue(q, (void**)&ptr);
		TEST_ASSERT(ptr == &data[i]);
	}
}

void test_queue_length(void)
{
	int data[] = {1, 2, 3, 4, 5};
	queue_t q;
	int i;

	fprintf(stderr, "*** TEST queue_length ***\n");

	q = queue_create();
	for (i = 0; i < 5; i++) {
		queue_enqueue(q, &data[i]);
	}

	TEST_ASSERT(queue_length(q) == 5);
}

void test_enqueue_null(void)
{
	queue_t q;

	fprintf(stderr, "*** TEST enqueue_null ***\n");

	q = queue_create();
	TEST_ASSERT(queue_enqueue(q, NULL) == -1);
}

void test_dequeue_empty(void)
{
	queue_t q;
	void *ptr = (void*)0xdead;

	fprintf(stderr, "*** TEST dequeue_empty ***\n");

	q = queue_create();
	TEST_ASSERT(queue_dequeue(q, &ptr) == -1 && ptr == (void*)0xdead);
}

void test_destroy_nonempty(void)
{
	int data = 3;
	queue_t q;

	fprintf(stderr, "*** TEST destroy_nonempty ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	TEST_ASSERT(queue_destroy(q) == -1);
}

int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_multiple();
	test_queue_length();
	test_enqueue_null();
	test_dequeue_empty();
	test_destroy_nonempty();

	return 0;
}
