#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

__attribute__((noreturn)) void error_handler(int errnum, char *err_function)
{
    char err_msg[250];
    strerror_r(errnum, err_msg, (size_t)250);
    printf("%s: %s\n", err_function, err_msg);
    exit(EXIT_FAILURE);
}

void *thread_callback_fn(void *_num_to_print)
{
    printf("%d\n", *((int *)_num_to_print));
    free(_num_to_print);
    return (void *)NULL;
}

void thread_create(pthread_t *thread_handle_pointer, int num_to_print)
{
    int errnum;
    pthread_attr_t attr;
    char err_msg[250];

    errnum = pthread_attr_init(&attr);
    if (errnum != 0)
    {
        error_handler(errnum, "pthread_attr_init");
    }

    errnum = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    if (errnum != 0)
    {
        error_handler(errnum, "pthread_attr_setdetachstate");
    }

    int *_num_to_print = (int *)malloc(sizeof(int));
    if (_num_to_print == (int *)NULL)
    {
        error_handler(errno, "malloc");
    }
    *_num_to_print = num_to_print;

    errnum = pthread_create(thread_handle_pointer, &attr, thread_callback_fn, (void *)_num_to_print);
    if (errnum != 0)
    {
        error_handler(errnum, "pthread_create");
    }

    errnum = pthread_attr_destroy(&attr);
    if (errnum != 0)
    {
        error_handler(errnum, "pthread_attr_destroy");
    }
}

pthread_t thread_handle;

int main(int argc, char **argv)
{
    int total_numbers = (argc > 1) ? (atoi(argv[1])) : 20;
    void *thread_return = malloc(sizeof(char));
    if (thread_return == (void *)NULL)
    {
        error_handler(errno, "malloc");
    }

    for (int i = total_numbers; i > 0; i--)
    {
        thread_create(&thread_handle, i);
        int errnum = pthread_join(thread_handle, &thread_return);

        if (errnum != 0)
        {
            error_handler(errnum, "pthread_join");
        }
    }
    free(thread_return);
}