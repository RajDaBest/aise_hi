#include "thread_barrier.h"
#include <stdlib.h>
#include <stdio.h>

#define N_THREADS 20

static pthread_t threads[N_THREADS];
thread_barrier_t thread_barrier;

void *thread_callback_fn(void *arg)
{
    int thread_id = (int)arg;
    int return_value = thread_barrier_wait(&thread_barrier, 5, thread_id);
    return NULL;
}

int main(int argc, char **argv)
{
    thread_barrier_init(&thread_barrier);
    for (size_t i = 0; i < (size_t)N_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, thread_callback_fn, (void *)i);
    }

    pthread_exit(EXIT_SUCCESS);
}