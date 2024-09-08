#ifndef _THREAD_BARRIER_
#define _THREAD_BARRIER_

#include <pthread.h>
#include <stdbool.h>

#define BLOCKED_THREAD_RELEASED 0    
#define THREAD_BARRIER_SERIAL_THREAD 1
#define NO_NEED_TO_BLOCK 2

typedef struct thread_barrier_
{
    pthread_mutex_t mutex;
    pthread_cond_t thread_wait_cond;
    pthread_cond_t disposition_cond;
    int num_of_threads_to_block;
    int num_of_threads_currently_blocked;
    bool in_disposition;
} thread_barrier_t;

void thread_barrier_init(thread_barrier_t* thread_barrier);
int thread_barrier_wait(thread_barrier_t* thread_barrier, int num_of_threads_to_block, int thread_id);
void thread_barrier_destroy(thread_barrier_t* thread_barrier);

#endif