#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define N_SLAVES 5

void cleanup_fd(void *arg)
{
    printf("%s invoked...\n", __FUNCTION__);
    int fd = *(int *)arg;
    close(fd);
}

void cleanup_pointer(void *arg)
{
    printf("%s invoked...\n", __FUNCTION__);
    free(arg);
}

void *
write_into_file(void *arg)
{
    char file_name[64];
    char string_to_write[64];
    int len;
    int count = 0;

    // mark the calling thread eligible for cancellation
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // or PTHREAD_CANCEL_DISABLE

    // mode of cancellation
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); // or PTHREAD_CANCEL_DEFERRED

    pthread_t thread = *(pthread_t *)arg;

    pthread_cleanup_push(cleanup_pointer, (void *)arg);

    sprintf(file_name, "Thread-%lu.txt", thread);

    int file_descriptor = open(file_name, O_TRUNC | O_CREAT | O_RDWR, S_IRWXG | S_IRWXO | S_IRWXU);

    pthread_cleanup_push(cleanup_fd, (void *)&file_descriptor);

    if (file_descriptor == -1)
    {
        perror("open(): ");
        goto cleanup;
    }

    while (1)
    {
        len = sprintf(string_to_write, "%d : I am thread %lu\n", count, thread);
        ssize_t write_return = write(file_descriptor, (void *)string_to_write, len);
        if (write_return == -1)
        {
            perror("write(): ");
            goto cleanup;
        }
        else if (0 <= write_return && write_return < len)
        {
            fprintf(stderr, "Not all bytes written to the file Thread-%lu.txt in count value %d", thread, count);
        }
        sleep(2);
        count++;

        pthread_testcancel();
    }

cleanup:

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(2);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_t *slaves[N_SLAVES];

    for (int j = 0; j < N_SLAVES; j++)
    {
        slaves[j] = (pthread_t *)malloc(sizeof(pthread_t));
    }

    int i;
    int *thread_id = 0;

    for (i = 0; i < N_SLAVES; i++)
    {
        thread_id = (int *)malloc(sizeof(int));
        *thread_id = i;

        pthread_create(slaves[i], NULL, write_into_file, (void *)thread_id);
    }

    int choice;
    int thread_num;

    while (1)
    {
        printf("1. Cancel the thread\n");
        scanf("%d", &choice);
        printf("Enter slave thread ID [0-%d] : ", N_SLAVES - 1);
        scanf("%d", &thread_num);
        if (thread_num < 0 || thread_num >= N_SLAVES)
        {
            printf("Invalid thread ID\n");
            exit(EXIT_FAILURE);
        }
        printf("\n");
        switch (choice)
        {
        case 1:
            pthread_cancel(*slaves[thread_num]);
            pthread_join(*slaves[thread_num], NULL);
            break;

        default:
            continue;
        }
    }
}