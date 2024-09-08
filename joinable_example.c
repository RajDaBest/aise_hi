#include <pthread.h> // for working with posix threads
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for pause () and sleep ()
#include <errno.h>  // for using global variable errno

void* thread_callback_fn (void* arg)
{
    int th_id = *((int*) arg);
    free (arg); // if the call to pthread_create returns an error, this function wont be executed; hence, we won't be freeing the same memory twice
    arg = NULL; 

    int rc = 0;
    while (rc < th_id)
    {
        printf ("Thread %d doing some work\n", th_id);
        sleep (1);
        rc++;
    }

    int* result = malloc (sizeof (int));
    *result = th_id * th_id;

    return (void*) result; 
}

pthread_t pthread2;
pthread_t pthread3;

void thread_create(pthread_t *pthread_handle, int th_id)
{
    char error_msg [250];
    int *_th_id = malloc(sizeof(int));
    *_th_id = th_id;

    pthread_attr_t attr;

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // PTHREAD_CREATE_DETACHED for detached threads
    int rc = pthread_create(pthread_handle, &attr, thread_callback_fn, (void *)_th_id);

    if (rc != 0)
    {
        strerror_r (rc, error_msg, (size_t) 250);
        printf ("pthread_create: %s", error_msg);
        free (_th_id);
        _th_id = NULL;
        exit (EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    thread_create(&pthread2, 10);
    thread_create(&pthread3, 2);

    void* thread1_result = malloc (sizeof (int));
    void* thread2_result = malloc (sizeof (int));

    printf ("main function blocked on pthread_join with th_id = 10\n");
    pthread_join (pthread2, &thread1_result);

    if (thread1_result != NULL) 
    {
        printf ("result returned from thread with th_id 10 = %d\n", *((int*) thread1_result));
        free (thread1_result); // always free memory after using
        thread1_result = NULL; // set dangling pointers to NULL
    }

    printf ("main function bloced on pthread_join with th_id = 2\n");
    pthread_join (pthread3, &thread2_result);

    if (thread2_result != NULL)
    {
        printf ("result returned from thread with th_id 2 = %d\n", *((int*) thread2_result));
        free (thread2_result);
        thread2_result = NULL;
    }
    return EXIT_SUCCESS;
}