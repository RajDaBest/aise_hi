#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

static int array[5] = {1, 2, 3, 4, 5};
static size_t len_array = sizeof(array) / sizeof(int);
pthread_mutex_t array_mutex;

void *_thread_callback_fn_swap(void *arg)
{
    int temp = 0;
    while (true)
    {
        pthread_mutex_lock(&array_mutex);
        temp = array[0];
        array[0] = array[len_array - 1];
        array[len_array - 1] = temp;
        pthread_mutex_unlock(&array_mutex);
    }
}

void swap_thread_create()
{
    pthread_t swap_thread;
    char err_msg[64];

    int return_value = pthread_create(&swap_thread, NULL, _thread_callback_fn_swap, NULL);
    if (return_value != 0)
    {
        strerror_r(return_value, err_msg, (size_t)64);
        fprintf(stderr, "pthread_create() : %s\n", err_msg);
        pthread_exit(NULL);
    }
}

void *_thread_callback_fn_sum(void *arg)
{
    int counter = 0, sum = 0;
    while (true)
    {
        pthread_mutex_lock(&array_mutex);
        for (; counter < len_array; counter++)
        {
            sum += array[counter];
        }
        pthread_mutex_unlock(&array_mutex);
        printf("sum is: %d\n", sum);
        sum = 0;
        counter = 0;
        sleep(1);
    }
}

void sum_thread_create()
{
    pthread_t sum_thread;
    char err_msg[64];

    int return_value = pthread_create(&sum_thread, NULL, _thread_callback_fn_sum, NULL);
    if (return_value != 0)
    {
        strerror_r(return_value, err_msg, (size_t)64);
        fprintf(stderr, "pthread_create() : %s\n", err_msg);
        pthread_exit(NULL);
    }
}

int main(int argc, char **argv)
{
    sum_thread_create();
    swap_thread_create();

    pthread_mutex_init(&array_mutex, NULL);

    pthread_exit(EXIT_SUCCESS);
}