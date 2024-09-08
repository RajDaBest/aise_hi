#include "thread_barrier.h"
#include <stdio.h>
void thread_barrier_init(thread_barrier_t *thread_barrier)
{
    thread_barrier->num_of_threads_currently_blocked = 0;
    thread_barrier->num_of_threads_to_block = 0;

    pthread_mutex_init(&thread_barrier->mutex, NULL);
    pthread_cond_init(&thread_barrier->thread_wait_cond, NULL);
    pthread_cond_init(&thread_barrier->disposition_cond, NULL);

    thread_barrier->in_disposition = false;
}

int thread_barrier_wait(thread_barrier_t *thread_barrier, int num_of_threads_to_block, int thread_id)
{
    pthread_mutex_lock(&thread_barrier->mutex);
    thread_barrier->num_of_threads_to_block = num_of_threads_to_block;
    while (thread_barrier->in_disposition)
    {
        fprintf(stdout, "Thread with id %d waiting on disposition\n", thread_id);
        pthread_cond_wait(&thread_barrier->disposition_cond, &thread_barrier->mutex);
    }

    if (thread_barrier->num_of_threads_currently_blocked + 1 == thread_barrier->num_of_threads_to_block)
    {
        fprintf(stdout, "Thread with id %d starting disposition\n", thread_id);
        thread_barrier->in_disposition = true;
        pthread_cond_signal(&thread_barrier->thread_wait_cond);
        pthread_mutex_unlock(&thread_barrier->mutex);
        return THREAD_BARRIER_SERIAL_THREAD;
    }

    thread_barrier->num_of_threads_currently_blocked++;
    fprintf(stdout, "Thread with id %d waiting on the barrier\n", thread_id);
    pthread_cond_wait(&thread_barrier->thread_wait_cond, &thread_barrier->mutex);
    thread_barrier->num_of_threads_currently_blocked--;

    if (thread_barrier->num_of_threads_currently_blocked == 0)
    {
        fprintf(stdout, "Thread with id %d is released and ends the disposition\n", thread_id);
        thread_barrier->in_disposition = false;
        pthread_cond_broadcast(&thread_barrier->disposition_cond);
    }
    else
    {
        fprintf(stdout, "Thread with id %d released during disposition and relays signal to a thread waiting on the barrier\n", thread_id);
        pthread_cond_signal(&thread_barrier->thread_wait_cond);
    }

    pthread_mutex_unlock(&thread_barrier->mutex);

    return BLOCKED_THREAD_RELEASED;
}

void thread_barrier_destroy(thread_barrier_t *thread_barrier)
{
    pthread_mutex_destroy(&thread_barrier->mutex);
    pthread_cond_destroy(&thread_barrier->disposition_cond);
    pthread_cond_destroy(&thread_barrier->thread_wait_cond);
}