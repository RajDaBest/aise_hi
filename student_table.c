#include "student_table.h"
#include <stdlib.h>
#include <stdbool.h>

void init_student_table_list()
{
    student_table_list = create_doubly_linked_list();
}

char check_student_entry_present(char _class, char roll_no)
{
    if (!check_table_present(_class))
    {
        return TABLE_DOES_NOT_EXIST;
    }

    student_table_t *table = return_table(_class);

    node_t *table_head = table->table->head;
    node_t *table_tail = table->table->tail;
    node_t *current_node = table_head->next_node;

    while (current_node != table_tail)
    {
        if (((student_table_entry_t *)(current_node->data))->roll_no == roll_no)
        {
            return ENTRY_EXISTS;
        }

        current_node = current_node->next_node;
    }

    return ENTRY_DOES_NOT_EXIST;
}

student_table_entry_t *return_student_entry(char _class, char roll_no)
{
    char return_value = check_student_entry_present(_class, roll_no);

    if (return_value != ENTRY_EXISTS)
    {
        return (student_table_entry_t *)return_value;
    }

    student_table_t *table = return_table(_class);

    node_t *table_head = table->table->head;
    node_t *table_tail = table->table->tail;
    node_t *current_node = table_head->next_node;

    while (current_node != table_tail)
    {
        if (((student_table_entry_t *)(current_node->data))->roll_no == roll_no)
        {
            return ((student_table_entry_t *)(current_node->data));
        }

        current_node = current_node->next_node;
    }
}

bool check_table_present(char _class)
{
    node_t *table_list_head = student_table_list->head;
    node_t *current_node = table_list_head->next_node;
    node_t *table_list_tail = student_table_list->tail;

    while (current_node != table_list_tail)
    {
        if (((student_table_t *)(current_node->data))->_class = _class)
        {
            return true;
        }

        current_node = current_node->next_node;
    }

    return false;
}

student_table_t *return_table(char _class)
{
    node_t *table_list_head = student_table_list->head;
    node_t *current_node = table_list_head->next_node;
    node_t *table_list_tail = student_table_list->tail;

    while (current_node != table_list_tail)
    {
        if (((student_table_t *)(current_node->data))->_class = _class)
        {
            return ((student_table_t *)(current_node->data));
        }

        current_node = current_node->next_node;
    }
}

student_table_t *create_student_table(char _class)
{
    student_table_t *new = (student_table_t *)malloc(sizeof(student_table_t));
    if (new == NULL)
    {
        return TABLE_CREATION_FAILURE;
    }

    new->_class = _class;
    new->table = create_doubly_linked_list();
    if (new->table == NULL)
    {
        return TABLE_CREATION_FAILURE;
    }

    insert_after_node(student_table_list->head, new, student_table_list);
    return new;
}

student_table_entry_t *create_student_table_entry(char *name, char *father_name, char *mother_name,
                                                  char roll_no, char marks_in_latest_test_maths,
                                                  char marks_in_latest_test_science, char num_of_siblings)
{
    student_table_entry_t *new = (student_table_entry_t *)malloc(sizeof(student_table_entry_t));
    new->name = name;
    new->father_name = father_name;
    new->mother_name = mother_name;
    new->roll_no = roll_no;
    new->marks_in_latest_test_maths = marks_in_latest_test_maths;
    new->marks_in_latest_test_science = marks_in_latest_test_science;
    new->num_of_siblings = num_of_siblings;

    char return_from_validate = validate_student_entry(new);

    if (return_from_validate != VALID_ENTRY)
    {
        return (student_table_entry_t *)return_from_validate;
    }

    return new;
}

char add_student_entry_to_table(char _class, student_table_entry_t *student_table_entry)
{
    if (!check_table_present(_class))
    {
        return TABLE_DOES_NOT_EXIST;
    }

    char validate_return = validate_student_entry(student_table_entry);
    if (validate_return != VALID_ENTRY)
    {
        return validate_return;
    }

    if (check_student_entry_present(_class, student_table_entry->roll_no) == ENTRY_EXISTS)
    {
        return ROLL_NO_ALREADY_TAKEN;
    }

    insert_at_beginning((void *)student_table_entry, return_table(_class));

    return SUCCESS;
}

char delete_student_entry_from_table(char _class, char roll_no)
{
    if (!check_table_present(_class))
    {
        return TABLE_DOES_NOT_EXIST;
    }

    if (check_student_entry_present(_class, roll_no) == ENTRY_DOES_NOT_EXIST)
    {
        return ENTRY_DOES_NOT_EXIST;
    }

    delete_node(return_student_entry(_class, roll_no), return_table(_class));
    return SUCCESS;
}

char update_student_entry_in_table(char _class, char roll_no, student_table_entry_t *data_to_update)
{
    char return_value = check_table_present(_class);
    if (!return_value)
    {
        return TABLE_DOES_NOT_EXIST;
    }

    return_value = check_student_entry_present(_class, roll_no);
    if (return_value != ENTRY_EXISTS)
    {
        return return_value;
    }

    student_table_entry_t *entry_to_update = return_student_entry(_class, roll_no);

    if (data_to_update->roll_no != NULL)
    {
        if (check_student_entry_present(_class, data_to_update->roll_no) == ENTRY_EXISTS)
        {
            return ROLL_NO_ALREADY_TAKEN;
        }
        else
        {
            entry_to_update->roll_no = data_to_update->roll_no;
        }
    }
    if (data_to_update->name != NULL)
    {
        entry_to_update->name = data_to_update->name;
    }
    if (data_to_update->father_name != NULL)
    {
        entry_to_update->father_name = data_to_update->father_name;
    }
    if (data_to_update->mother_name != NULL)
    {
        entry_to_update->mother_name = data_to_update->mother_name;
    }
    if (data_to_update->num_of_siblings != NULL)
    {
        entry_to_update->num_of_siblings = data_to_update->num_of_siblings;
    }
    if (data_to_update->marks_in_latest_test_maths != NULL)
    {
        entry_to_update->marks_in_latest_test_maths = data_to_update->marks_in_latest_test_maths;
    }
    if (data_to_update->marks_in_latest_test_science != NULL)
    {
        entry_to_update->marks_in_latest_test_science = data_to_update->marks_in_latest_test_science;
    }

    return SUCCESS;
}

char delete_student_table(char _class)
{
    if (!check_table_present(_class))
    {
        return TABLE_DOES_NOT_EXIST;
    }

    student_table_t *table = return_table(_class);

    node_t *list_head = student_table_list->head;
    node_t *list_tail = student_table_list->tail;
    node_t *current_node = list_head->next_node;

    while (current_node != list_tail)
    {
        if (((student_table_t *)(current_node->data))->_class = _class)
        {
            delete_node(current_node, student_table_list);
            break;
        }
    }

    delete_doubly_linked_list(table->table);
    free(table);
}

char validate_student_entry(student_table_entry_t *entry_to_validate)
{
    char roll_no = (char)entry_to_validate->roll_no;
    char math_marks = (char)entry_to_validate->marks_in_latest_test_maths;
    char science_marks = (char)entry_to_validate->marks_in_latest_test_science;
    char num_of_siblings = (char)entry_to_validate->num_of_siblings;
    if (roll_no > 50 || roll_no < 1)
    {
        return INVALID_ROLL_NO;
    }
    else if (math_marks > 100 || math_marks < -100 || science_marks > 100 || science_marks < -100)
    {
        return INVALID_MARKS;
    }
    else if (num_of_siblings < 0)
    {
        return INVALID_NUMBER_OF_SIBLINGS;
    }
    else
    {
        return VALID_ENTRY;
    }
}
