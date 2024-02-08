#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct queue {
	/* TODO Phase 1 */
	struct Node {
		void *data; //Allows for any data type
		struct Node *next;
	} *head, *tail;

	int size;
};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t queue = malloc(sizeof(struct queue));
	if (queue == NULL)
	{
		return NULL;
	}

	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;

	return queue;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	
	if (queue == NULL || queue->head != NULL)
	{
		return -1;
	}

	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */

	if (queue == NULL || data == NULL)
	{
		return -1;
	}

	// Dynamically allocated memory for node struct of linked list
	Node *newNode = malloc(sizeof(Node));

	if (newNode == NULL)
	{
		return -1;
	}

	newNode->data = data;
	newNode->next = NULL:

	// If queue is empty, newNode is both head and tail.
	if (queue->head == NULL)
	{
		queue->head = newNode;
		queue->tail = newNode;
	}
	// Else, set pointer to next node and makes newNode the tail.
	else
	{
		queue->tail->next = newNode;
		queue->tail = newNode;
	}

	queue->size++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */

	if (queue == NULL || data == NULL || queue->head == NULL)
	{
		return -1;
	}

	Node *temp = queue->head; //temp value to hold head value
	*data = queue->head->data;

	// If queue only has one element
	if (queue->head == queue->tail)
	{
		queue->head == NULL;
		queue->tail == NULL;
	}
	// If more than one element, head pointer updated to next node
	else
	{
		queue->head = queue->head->next;
	}

	free(temp);
	queue->size--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	if (queue != NULL)
	{
		return -1;
	}

	return queue->size;
}

