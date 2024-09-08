#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

int queueElementCount(queue_t *queue)
{
    return queue->count;
}

bool isFull(queue_t *queue)
{
    return queue->count == MAX_QUEUE_SIZE;
}

bool isEmpty(queue_t *queue)
{
    return queue->count == 0;
}

queue_t *createQueue()
{
    queue_t *new_queue = (queue_t *)malloc(sizeof(queue_t));
    if (new_queue == NULL)
    {
        return (queue_t *)QUEUE_CREATION_ERROR;
    }

    new_queue->front = (queue_node_t *)malloc(sizeof(queue_node_t));
    if (new_queue->front == NULL)
    {
        free(new_queue);
        return (queue_t *)QUEUE_CREATION_ERROR;
    }

    new_queue->rear = (queue_node_t *)malloc(sizeof(queue_node_t));
    if (new_queue->rear == NULL)
    {
        free(new_queue->front);
        free(new_queue);
        return (queue_t *)QUEUE_CREATION_ERROR;
    }

    new_queue->front->next_node = new_queue->rear;
    new_queue->rear->previous_node = new_queue->front;
    new_queue->front->previous_node = NULL;
    new_queue->rear->next_node = NULL;
    new_queue->count = 0;

    return new_queue;
}

int enqueue(queue_t *queue, int element)
{
    if (isFull(queue))
    {
        return QUEUE_FULL;
    }

    queue_node_t *new_queue_node = (queue_node_t *)malloc(sizeof(queue_node_t));
    if (new_queue_node == NULL)
    {
        return ENQUEUE_FAILURE;
    }

    new_queue_node->data = element;
    new_queue_node->next_node = queue->rear;
    new_queue_node->previous_node = queue->rear->previous_node;
    queue->rear->previous_node->next_node = new_queue_node;
    queue->rear->previous_node = new_queue_node;
    (queue->count)++;
    return ENQUEUE_SUCCESS;
}

int dequeue(queue_t *queue)
{
    if (isEmpty(queue))
    {
        return QUEUE_EMPTY;
    }

    queue_node_t *node_to_be_deleted = queue->front->next_node;
    queue->front->next_node = node_to_be_deleted->next_node;
    if (queue->front->next_node != NULL)
    {
        queue->front->next_node->previous_node = queue->front;
    }
    (queue->count)--;

    free(node_to_be_deleted);
    return DEQUEUE_SUCCESS;
}

int destroyQueue(queue_t *queue)
{
    if (queue == NULL)
    {
        return DESTROY_QUEUE_FAILURE;
    }

    queue_node_t *current_node = queue->front;
    while (current_node != NULL)
    {
        queue_node_t *temp = current_node;
        current_node = current_node->next_node;
        free(temp);
    }

    free(queue);
    return DESTROY_QUEUE_SUCCESS;
}
