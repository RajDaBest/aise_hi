#ifndef _STUDENT_TABLE
#define _STUDENT_TABLE

#include <stdbool.h>
#include "doubly_linked_list.h"
#define TABLE_DOES_NOT_EXIST 2
#define ENTRY_DOES_NOT_EXIST 3
#define INVALID_MARKS 4
#define INVALID_ROLL_NO 5
#define INVALID_NUMBER_OF_SIBLINGS 6
#define VALID_ENTRY 7
#define TABLE_CREATION_FAILURE ((student_table_t *)NULL)
#define ENTRY_EXISTS 8
#define ROLL_NO_ALREADY_TAKEN 9

doubly_linked_list_t *student_table_list;

typedef struct
{
    doubly_linked_list_t *table;
    char _class;
} student_table_t;

typedef struct
{
    char *name;
    char *father_name;
    char *mother_name;
    char roll_no;                    // roll_no is from 1 to 50
    char marks_in_latest_test_maths; // marks can range from -100 to 100
    char marks_in_latest_test_science;
    char num_of_siblings;
} student_table_entry_t;

char check_student_entry_present(char _class, char roll_no);
student_table_entry_t *return_student_entry(char _class, char roll_no);
bool check_table_present(char _class);
student_table_t *return_table(char _class);
void init_student_table_list();
char validate_student_entry(student_table_entry_t *entry_to_validate);
student_table_t *create_student_table(char _class);
student_table_entry_t *create_student_table_entry(char *name, char *father_name, char *mother_name,
                                                  char roll_no, char marks_in_latest_test_maths,
                                                  char marks_in_latest_test_science, char num_of_siblings);

char add_student_entry_to_table(char _class, student_table_entry_t *student_table_entry);
char delete_student_entry_from_table(char _class, char roll_no);
char update_student_entry_in_table(char _class, char roll_no, student_table_entry_t *data_to_update); // fields other than the ones that are
                                                                                                      // to be updated should be set to NULL in data_to_update

char delete_student_table(char _class);

#endif
