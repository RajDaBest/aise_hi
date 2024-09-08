#ifndef _QUEUE_
#define _QUEUE_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NULL_INT 0
#define QUEUE_EMPTY 1
#define QUEUE_CREATION_ERROR 2
#define ENQUEUE_FAILURE 3
#define ENQUEUE_SUCCESS 4
#define DEQUEUE_SUCCESS 5
#define DESTROY_QUEUE_SUCCESS 6
#define QUEUE_FULL 7
#define DESTROY_QUEUE_FAILURE 8
#define MAX_QUEUE_SIZE 10
#define QUEUE_INITIALIZER createQueue()
#define RANDOM_INTEGER rand()
#define peekQueue(queue) queue->front->next_node->data

typedef struct queue_node
{
    struct queue_node *previous_node;
    struct queue_node *next_node;
    int data;
} queue_node_t;

typedef struct queue
{
    queue_node_t *front;
    queue_node_t *rear;
    int count;
} queue_t;

bool isFull(queue_t *queue);
bool isEmpty(queue_t *queue);
queue_t *createQueue();
int dequeue(queue_t *queue);
int enqueue(queue_t *queue, int element);
int destroyQueue(queue_t *queue);
int queueElementCount(queue_t *queue);

#endif