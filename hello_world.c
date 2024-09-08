#include <pthread.h> // for working with POSIX threads
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h> // for pause () and sleep ()

// A thread callback function must have following prototypes:

static void* thread_fn_callback (void* arg)
{
    char* input = (char*) arg;

    char a = 0;
    
    while (a < 10)
    {
        printf ("input string = %s\n", input);
        sleep (1);
        a++;
    }
}

void thread1_create ()
{
    pthread_t pthread1; // thread handle

    // Take some argument to be passed to the thread function,
    // look after that you always pass the persistent memory
    // as an argument to the thread, do not pass caller's
    // local variables or stack memory

    static char* thread_input1 = "I am thread no 1";
    char error_msg [250];

    // Returns 0 on success, otherwise returns errorcode; all pthread
    // functions return the corresponding errno error code on failure;
    // they do not set global errno variable

    int rc = pthread_create (&pthread1, NULL, thread_fn_callback, (void*) thread_input1);

    if (rc != 0)
    {
        strerror_r (-rc, error_msg, (size_t) 250); 
        printf ("pthread_create: %s\n", error_msg);
        exit (EXIT_FAILURE);
    }
}

int main (int argc, char** argv) 
{
    thread1_create ();
    // printf ("main fn paused\n");
    // pause (); // if the main thread terminates, then all the child threads in the program get terminated automatically; so we pause the main thread here
    // if we use pthread_exit (0) to terminate the main thread, then all other child threads of the process won't get terminated automatically; the process will terminate when the last executing
    // child thread terminates

    // termination of a child thread doesn't effect any other threads of the process  
    pthread_exit (0);

    printf ("hello"); // this won't be printed since the main thread has exited
    // return EXIT_SUCCESS;
}