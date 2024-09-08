/* #include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

pthread_t worker_one;
pthread_t worker_two;
pthread_t worker_three;

typedef struct
{
    int start;
    int end;
} parameters;

int file_descriptor;

void *thread_callback_fn(void *local_parameters)
{
    parameters *_local_parameters = (parameters *)local_parameters;
    int file_descriptor = open("random.txt", O_RDONLY, S_IRUSR | S_IRGRP | S_IROTH);
    if (file_descriptor != 0)
    {
        perror("open: ");
        exit(EXIT_FAILURE);
    }

    char buf;

    int line_count = 0;
    int* word_count = malloc (sizeof (int));
    *word_count = 0;

    while (line_count < _local_parameters->end)
    {
        while (buf != '\n')
        {
            int read_result = read(file_descriptor, (void *)buf, (size_t)1);
            if (read_result == -1)
            {
                perror("read: ");
                exit(EXIT_FAILURE);
            }

            if (read_result == 0)
            {
                return (void *)word_count;
            }

            if (line_count >= _local_parameters->start)
            {
                if (buf == ' ')
                {
                    (*word_count)++;
                }
            }
        }

        line_count++;
    }

    free (_local_parameters);
    return (void *) word_count;
}

void thread_create(pthread_t *restrict thread_handle_pointer, int local_start, int local_end)
{
    pthread_attr_t attr;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    parameters *local_parameters = malloc(sizeof(parameters));

    local_parameters->start = local_start;
    local_parameters->end = local_end;

    pthread_create(thread_handle_pointer, &attr, thread_callback_fn, (void *)local_parameters);
}

int main(int argc, char **argv)
{
    thread_create(&worker_one, 0, 399);
    thread_create(&worker_two, 400, 799);
    thread_create(&worker_three, 799, 1199);

    void *result_one, *result_two, *result_three;

    pthread_join(worker_one, &result_one);
    pthread_join(worker_two, &result_two);
    pthread_join(worker_three, &result_three);

    free (result_one);
    free (result_two);
    free (result_three);

    int num_of_words = *((int *)result_one) + *((int *)result_two) + *((int *)result_three);
    printf("number of words in the file is: %d", num_of_words);
    return EXIT_SUCCESS;
} */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

pthread_t worker_one;
pthread_t worker_two;
pthread_t worker_three;

typedef struct
{
    int start;
    int end;
    char* file;
} parameters;

int file_descriptor;

void *thread_callback_fn(void *local_parameters)
{
    parameters *_local_parameters = (parameters *)local_parameters;
    int file_descriptor = open(_local_parameters -> file, O_RDONLY);
    if (file_descriptor == -1)
    {
        perror("open: ");
        exit(EXIT_FAILURE);
    }

    char buf;
    int line_count = 0;
    int *word_count = malloc(sizeof(int));
    *word_count = 0;

    lseek(file_descriptor, 0, SEEK_SET);

    while (line_count < _local_parameters->end)
    {
        ssize_t read_result = read(file_descriptor, &buf, 1);
        if (read_result == -1)
        {
            perror("read: ");
            exit(EXIT_FAILURE);
        }

        if (read_result == 0)
        {   
            if (_local_parameters -> start >= 799)
            {
                (*word_count)++;
            }
            break; // End of file reached
        }

        if (line_count >= _local_parameters->start)
        {
             if (buf == ' ')
            {
                (*word_count)++;
            }
        }

        if (buf == '\n')
        {
            line_count++;
        }
    }

    close(file_descriptor);
    free(_local_parameters);
    return (void *)word_count;
}

void thread_create(pthread_t *restrict thread_handle_pointer, int local_start, int local_end, char* restrict file_path)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    parameters *local_parameters = malloc(sizeof(parameters));
    local_parameters->start = local_start;
    local_parameters->end = local_end;
    local_parameters->file = file_path;

    pthread_create(thread_handle_pointer, &attr, thread_callback_fn, (void *)local_parameters);
    pthread_attr_destroy(&attr);
}

int main(int argc, char **argv)
{
    char* file_path = (argc > 1) ? argv [1] : "random.txt";
    
    thread_create(&worker_one, 0, 399, file_path);
    thread_create(&worker_two, 400, 799, file_path);
    thread_create(&worker_three, 800, 1199, file_path);

    void *result_one, *result_two, *result_three;

    pthread_join(worker_one, &result_one);
    pthread_join(worker_two, &result_two);
    pthread_join(worker_three, &result_three);

    int num_of_words = *((int *)result_one) + *((int *)result_two) + *((int *)result_three);
    printf("Number of words in the file is: %d\n", num_of_words);

    free(result_one);
    free(result_two);
    free(result_three);

    return EXIT_SUCCESS;
}
