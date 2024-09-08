#include "doubly_linked_list.h"
#include <stdlib.h>
#include <stdbool.h>

struct doubly_linked_list *create_doubly_linked_list()
{
    struct doubly_linked_list *new = (struct doubly_linked_list *)malloc(sizeof(struct doubly_linked_list));
    struct node *head = create_new_node(0);
    struct node *tail = create_new_node(0);

    if (new == NULL || head == NULL || tail == NULL)
    {
        return (struct doubly_linked_list *)NULL;
    }

    head->next_node = tail;
    tail->previous_node = head;

    new->head = head;
    new->tail = tail;

    return new;
}

struct node *create_new_node(void* data)
{
    struct node *new = (struct node *)malloc(sizeof(struct node));
    if (new == (struct node *)NULL)
    {
        return new;
    }

    new->data = data;
    new->next_node = (struct node *)NULL;
    new->previous_node = (struct node *)NULL;

    return new;
}

int insert_at_beginning(void* data, struct doubly_linked_list *list)
{
    if (list == (struct doubly_linked_list *)NULL)
    {
        return INVALID_LIST;
    }
    struct node *new = create_new_node(data);
    struct node *previous_first_node = (list->head)->next_node;

    new->next_node = previous_first_node;
    new->previous_node = list->head;
    previous_first_node->previous_node = new;
    (list->head)->next_node = new;

    return SUCCESS;
}

int insert_at_end(void* data, struct doubly_linked_list *list)
{
    if (list == (struct doubly_linked_list *)NULL)
    {
        return INVALID_LIST;
    }
    struct node *new = create_new_node(data);
    struct node *previous_last_node = (list->tail)->previous_node;

    if (new == (struct node *)NULL)
    {
        return FAILURE;
    }

    (list->tail)->previous_node = new;
    new->next_node = list->tail;
    new->previous_node = previous_last_node;
    previous_last_node->next_node = new;

    return SUCCESS;
}

int insert_after_node(struct node *node_to_insert_after, void* data, struct doubly_linked_list *list)
{
    if (list == (struct doubly_linked_list *)NULL)
    {
        return INVALID_LIST;
    }
    if (node_present(node_to_insert_after, list) == NODE_NOT_FOUND)
    {
        return NODE_NOT_FOUND;
    }
    struct node *temp = node_to_insert_after->next_node;
    struct node *new = create_new_node(data);

    if (new == (struct node *)NULL)
    {
        return FAILURE;
    }

    new->next_node = temp;
    new->previous_node = node_to_insert_after;
    node_to_insert_after->next_node = new;
    temp->previous_node = new;

    return SUCCESS;
}

int insert_before_node(struct node *node_to_insert_before, void* data, struct doubly_linked_list *list)
{
    if (list == (struct doubly_linked_list *)NULL)
    {
        return INVALID_LIST;
    }
    if (node_present(node_to_insert_before, list) == NODE_NOT_FOUND)
    {
        return NODE_NOT_FOUND;
    }
    struct node *temp = node_to_insert_before->previous_node;
    struct node *new = create_new_node(data);

    if (new == (struct node *)NULL)
    {
        return FAILURE;
    }

    node_to_insert_before->previous_node = new;
    temp->next_node = new;
    new->next_node = node_to_insert_before;
    new->previous_node = temp;

    return SUCCESS;
}

int delete_node(struct node *node_to_be_deleted, struct doubly_linked_list *list)
{
    if (list == (struct doubly_linked_list *)NULL)
    {
        return INVALID_LIST;
    }
    if (node_to_be_deleted == list->head)
    {
        return CANNOT_DELETE_HEAD;
    }

    if (node_to_be_deleted == list->tail)
    {
        return CANNOT_DELETE_TAIL;
    }
    struct node *current_node = list->head;
    char not_found = true;
    if (node_present(node_to_be_deleted, list) == NODE_FOUND)
    {
        (node_to_be_deleted->previous_node)->next_node = (node_to_be_deleted)->next_node;
        (node_to_be_deleted->next_node)->previous_node = (node_to_be_deleted)->previous_node;

        free(node_to_be_deleted->data);
        free(node_to_be_deleted->next_node);
        free(node_to_be_deleted->previous_node);

        free(node_to_be_deleted);
        return SUCCESS;
    }

    return NODE_NOT_FOUND;
}

int delete_doubly_linked_list(struct doubly_linked_list *list)
{
    if (list == (struct doubly_linked_list *)NULL)
    {
        return INVALID_LIST;
    }
    struct node *current_node = (list->head)->next_node;
    struct node *temp;

    while (current_node != list->tail)
    {
        temp = current_node->next_node;
        delete_node(current_node, list);
        current_node = temp;
    }

    free(list->head);
    free(list->tail);
    free(list);

    return SUCCESS;
}

/* struct node *search(int data, struct doubly_linked_list *list)
{
    if (list == (struct doubly_linked_list *)NULL)
    {
        return SEARCH_INVALID_LIST;
    }
    struct node *current_node = (list->head)->next_node;

    while (current_node->next_node != list->tail)
    {
        if ((current_node->data) == data)
        {
            return current_node;
        }

        current_node = current_node->next_node;
    }

    return SEARCH_NODE_NOT_FOUND;
} */

int node_present(struct node *node_to_find, struct doubly_linked_list *list)
{
    if (list == (struct doubly_linked_list *)NULL)
    {
        return INVALID_LIST;
    }
    struct node *current_node = (list->head)->next_node;

    while (current_node->next_node != list->tail)
    {
        if (current_node == node_to_find)
        {
            return NODE_FOUND;
        }
    }

    return NODE_NOT_FOUND;
}