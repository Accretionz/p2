#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct node {
    void *data;
    struct node *next;
} Node;

struct queue {
    int size;
    Node *head;
    Node *tail;
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
	newNode->next = NULL;

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
		queue->head = NULL;
		queue->tail = NULL;
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

	if (queue == NULL || data || NULL || queue->head == NULL)
	{
		return -1;
	}

	Node *current = queue->head;
	Node *previous = NULL;

	while (current != NULL)
	{
		if (current->data == data)
		{
			if (previous == NULL)
			{
				queue->head = current->next;
			}
			else
			{
				previous->next = current->next;
			}

			if (current == queue->tail)
			{
				queue->tail = previous;
			}

			free(current);
			queue->size--;
			return 0;
		}

		previous = current;
		current = current->next;
	}

	return -1;
}


int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */

	if (queue == NULL || func == NULL)
	{
		return -1;
	}

	Node *current = queue->head;

	while (current != NULL)
	{
		Node *temp = current->next;

		func(queue, current->data);

		current = temp;
	}

	return 0;
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

