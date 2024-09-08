#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_cond_full = PTHREAD_COND_INITIALIZER;
queue_t *queue;

void handlePthreadError(int error_value, char *function_name)
{
    if (error_value != 0)
    {
        char err_msg[64];
        strerror_r(error_value, err_msg, 64);
        fprintf(stderr, "%s : %s\n", function_name, err_msg);
        exit(EXIT_FAILURE);
    }
}

void *producerCallbackFunction(void *thread_no_)
{
    int return_value;

    while (true)
    {
        fprintf(stdout, "producer thread no %d has been scheduled\n", (int) thread_no_);
        return_value = pthread_mutex_lock(&queue_mutex);
        handlePthreadError(return_value, "pthread_mutex_lock()");
        fprintf(stdout, "producer thread no %d is granted the mutex\n", (int) thread_no_);

        while (isFull(queue))
        {
            return_value = pthread_cond_broadcast(&queue_cond_empty);
            handlePthreadError(return_value, "pthread_cond_broadcast()");
            return_value = pthread_cond_wait(&queue_cond_full, &queue_mutex);
            handlePthreadError(return_value, "pthread_cond_wait()");
        }
        while (queue->count < MAX_QUEUE_SIZE)
        {
            int random = RANDOM_INTEGER;
            fprintf(stdout, "producer thread number %d enqueued element %d\n", (int)thread_no_, random);
            enqueue(queue, random);
        }
        return_value = pthread_mutex_unlock(&queue_mutex);
        fprintf(stdout, "producer thread no %d has unlocked the mutex\n", (int) thread_no_);
        handlePthreadError(return_value, "pthread_mutex_unlock");
        sleep(1);
    }
}

void createProducerThread(int thread_no)
{
    pthread_attr_t attr;
    pthread_t producer_thread;
    char err_msg[64];
    int return_value;

    return_value = pthread_attr_init(&attr);
    handlePthreadError(return_value, "pthread_attr_init()");

    return_value = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    handlePthreadError(return_value, "pthread_attr_setdetachstate()");

    return_value = pthread_create(&producer_thread, &attr, producerCallbackFunction, (void *)thread_no);
    handlePthreadError(return_value, "pthread_create()");

    return_value = pthread_attr_destroy(&attr);
    handlePthreadError(return_value, "pthread_attr_destroy");
}

void *consumerCallbackFunction(void *thread_no_)
{
    int return_value;

    while (true)
    {
        fprintf(stdout, "consumer thread no %d w has been scheduled\n", (int) thread_no_);
        return_value = pthread_mutex_lock(&queue_mutex);
        handlePthreadError(return_value, "pthread_mutex_lock()");
        fprintf(stdout, "consumer thread no %d is granted the mutex\n", (int) thread_no_);
        while (isEmpty(queue))
        {
            return_value = pthread_cond_broadcast(&queue_cond_full);
            handlePthreadError(return_value, "pthread_cond_broadcast()");
            return_value = pthread_cond_wait(&queue_cond_empty, &queue_mutex);
            handlePthreadError(return_value, "pthread_cond_wait()");
        }
        while (queue->count > 0)
        {
            fprintf(stdout, "consumer thread number %d dequeued element %d\n", (int)thread_no_, peekQueue(queue));
            dequeue(queue);
        }
        return_value = pthread_mutex_unlock(&queue_mutex);
        fprintf(stdout, "consumer thread no %d has unlocked the mutex\n", (int) thread_no_);
        handlePthreadError(return_value, "pthread_mutex_unlock");
        sleep(1);
    }
}

void createConsumerThread(int thread_no)
{
    pthread_attr_t attr;
    pthread_t consumer_thread;
    char err_msg[64];
    int return_value;

    return_value = pthread_attr_init(&attr);
    handlePthreadError(return_value, "pthread_attr_init()");

    return_value = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED );
    handlePthreadError(return_value, "pthread_attr_setdetachstate()");

    return_value = pthread_create(&consumer_thread, &attr, consumerCallbackFunction, (void *)thread_no);
    handlePthreadError(return_value, "pthread_create()");

    return_value = pthread_attr_destroy(&attr);
    handlePthreadError(return_value, "pthread_attr_destroy");
}

int main(int argc, char **argv)
{
    queue = QUEUE_INITIALIZER;

    createConsumerThread(0);
    createConsumerThread(1);
    createProducerThread(0);
    createProducerThread(1);

    pthread_exit(EXIT_SUCCESS);
}