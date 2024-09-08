#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N_THREADS 20

static pthread_t threads[N_THREADS];
pthread_barrier_t thread_barrier;

void *thread_callback_fn(void *arg)
{
    int thread_id = (int)(size_t)arg;

    // Wait for the barrier
    int return_value = pthread_barrier_wait(&thread_barrier);

    if (return_value == PTHREAD_BARRIER_SERIAL_THREAD)
    {
        fprintf(stdout, "Thread no %d released the barrier (PTHREAD_BARRIER_SERIAL_THREAD)\n", thread_id);
    }
    else
    {
        fprintf(stdout, "Thread no %d was waiting and released\n", thread_id);
    }

    return NULL;
}

int main(int argc, char **argv)
{
    // Initialize the barrier to block 19 threads
    pthread_barrier_init(&thread_barrier, NULL, 19);

    // Create threads
    for (size_t i = 0; i < N_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, thread_callback_fn, (void *)i);
    }

    // Wait for all threads to finish
    for (size_t i = 0; i < N_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy the barrier
    pthread_barrier_destroy(&thread_barrier);

    return EXIT_SUCCESS;
}
