#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void *fn(void *arg)
{
    printf("process ID: %d\n", getpid());
    printf("thread ID: %lu\n", (unsigned long)pthread_self());
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t hello;
    printf("process ID: %d\n", getpid());
    printf("thread ID: %lu\n", (unsigned long)pthread_self());
    pthread_create(&hello, NULL, fn, NULL);
    pthread_exit(NULL);
}