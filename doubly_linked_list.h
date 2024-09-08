#ifndef __DOUBLY_LINKED_LIST
#define __DOUBLY_LINKED_LIST

#define SUCCESS 0
#define FAILURE 1
#define NODE_NOT_FOUND 2
#define CANNOT_DELETE_HEAD 3
#define CANNOT_DELETE_TAIL 4
// #define SEARCH_NODE_NOT_FOUND ((struct node*) NULL)
#define NODE_FOUND 5
#define NULL_NODE_POINTER 6
#define INVALID_LIST 7
// #define SEARCH_INVALID_LIST ((struct node*) INVALID_LIST)

typedef struct node
{
    void *data;
    struct node *next_node;
    struct node *previous_node;
} node_t;

typedef struct doubly_linked_list  
{
    struct node *head;
    struct node *tail;
} doubly_linked_list_t;

struct doubly_linked_list *create_doubly_linked_list();
struct node *create_new_node(void *data);
int insert_after_node(struct node *node_to_insert_after, void *data, struct doubly_linked_list *list);
int insert_before_node(struct node *node_to_insert_before, void *data, struct doubly_linked_list *list);
int insert_at_beginning(void* data, struct doubly_linked_list *list);
int insert_at_end(void* data, struct doubly_linked_list *list);
int delete_node(struct node *node_to_be_deleted, struct doubly_linked_list *list);
int delete_doubly_linked_list(struct doubly_linked_list *list);
// struct node *search(int data, struct doubly_linked_list* list);
int node_present(struct node *node_to_find, struct doubly_linked_list *list);

#endif