#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"

queue_t *queue;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_condition = PTHREAD_COND_INITIALIZER;

void *consumer_callback_fn(void *arg)
{
    while (true)
    {
        pthread_mutex_lock(&queue_mutex);
        while (isEmpty(queue))  // while prevents spurious wake ups 
        {
            pthread_cond_wait(&queue_condition, &queue_mutex);
        }

        fprintf(stdout, "Element at rear end of the queue: %d\n", queue->rear->previous_node->data);
        dequeue(queue);
        pthread_mutex_unlock(&queue_mutex);
        sleep(1);
    }
}

void *producer_callback_fn(void *arg)
{
    int counter = 0;
    while (true)
    {
        pthread_mutex_lock(&queue_mutex);
        fprintf(stdout, "Added %d to the front of the queue\n", counter);
        enqueue(queue, counter++);
        pthread_cond_signal(&queue_condition);
        pthread_mutex_unlock(&queue_mutex);
        sleep(1);
    }
}

void create_producer_thread()
{
    pthread_t producer_thread;
    char err_msg[64];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int return_value = pthread_create(&producer_thread, &attr, producer_callback_fn, NULL);
    pthread_attr_destroy(&attr);
    if (return_value != 0)
    {
        strerror_r(return_value, err_msg, 64);
        fprintf(stderr, "pthread_create() : %s\n", err_msg);
        exit(EXIT_FAILURE);
    }
}

void create_consumer_thread()
{
    pthread_t consumer_thread;
    char err_msg[64];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int return_value = pthread_create(&consumer_thread, &attr, consumer_callback_fn, NULL);
    pthread_attr_destroy(&attr);
    if (return_value != 0)
    {
        strerror_r(return_value, err_msg, 64);
        fprintf(stderr, "pthread_create() : %s\n", err_msg);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    queue = createQueue();
    if (queue == (queue_t *)QUEUE_CREATION_ERROR)
    {
        fprintf(stderr, "Queue Creation Error!\n");
        exit(EXIT_FAILURE);
    }

    create_consumer_thread();
    create_producer_thread();

    pthread_exit((void *)EXIT_SUCCESS);
}
