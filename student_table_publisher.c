#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "student_table.h"

student_table_t *student_table;

void main_menu ()
{
    int choice;
    while (1)
    {
        printf ("Publisher Menu\n");
        printf ("0. Update student table entry");
        printf ("1. Add student table entry\n");
        printf ("2. Delete table entry\n");
        printf ("3. Delete student table\n");
        printf ("Enter Choice: ");
        choice = 0;
        scanf ("%d", &choice);
        switch (choice)
        {
        case 0:
            printf ("Enter the roll no of the student whose entry you want to update: ");
            char roll_no;
            scanf ("%d", &roll_no);

            if (check_student_entry_present (1, roll_no) != ENTRY_DOES_NOT_EXIST)
            {
                printf ("Student with the specified roll no does not exist\n");
            }

            printf ("Do you want to update latest math marks (1) or latest science marks (2) or both (3)?\n");
            scanf ("%d", &choice);

            if (choice == 1)
            {
                
            }
            
            break;

        case 1:

            break;

        case 2:

            break;

        case 3:

            break;
        
        default:
            break;
        }
    }
}

void *publisher_thread_fn(void *arg)
{
    // add some default entries in the student table
    student_table_entry_t *entry = create_student_table_entry("raj", "arvind", "ranjeeta", 21, 100, 100, 2);
    add_student_entry_to_table(1, entry);

    entry = create_student_table_entry("pari", "arvind", "ranjeeta", 16, 80, 80, 2);
    add_student_entry_to_table(1, entry);

    entry = create_student_table_entry("angel", "arvind", "ranjeeta", 2, 90, 90, 2);
    add_student_entry_to_table(1, entry);

    entry = create_student_table_entry("aaradhya", "santosh", "deepa", 1, 95, 95, 0);
    add_student_entry_to_table(1, entry);
    main_menu ();
}

void create_publisher_thread()
{
    pthread_attr_t attr;
    pthread_t pub_thread;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&pub_thread, &attr, publisher_thread_fn, (void *)0);
}

int main(int argc, char **argv)
{
    init_student_table_list();
    student_table = create_student_table(1);
    // create subscriber threads

    create_subscriber_thread(1);
    sleep(1);

    create_subscriber_thread(2);
    sleep(1);

    create_subscriber_thread(3);
    sleep(1);

    // create publisher thread
    create_publisher_thread();
    printf("Publisher thread created!\n");

    pthread_exit(0);
}