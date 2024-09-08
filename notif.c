#include "notif.h"
#include <string.h>
#include <stdlib.h>

int nfc_register_notif_chain(notif_chain_t *nfc, notif_chain_element_data_t *nfce)
{
    int result = insert_after_node((nfc->notif_chain)->head, (void *)nfce, nfc->notif_chain);
    if (result == FAILURE)
    {
        return FAILURE;
    }
    return SUCCESS;
}

int nfc_invoke_notif_chain(notif_chain_t *nfc, void *arg, size_t arg_size, char *key, size_t key_size, nfc_op_t* _nfc_op)
{
    key[key_size] = '\n';
    doubly_linked_list_t *notif_chain = nfc->notif_chain;
    node_t *notif_chain_head = notif_chain->head;
    node_t *notif_chain_tail = notif_chain->tail;
    node_t *current_node = notif_chain_head->next_node;

    if (notif_chain_head->next_node == notif_chain_tail)
    {
        return EMPTY_NOTIF_CHAIN;
    }

    while (current_node != notif_chain_tail)
    {
        notif_chain_element_data_t *current_node_data = (notif_chain_element_data_t *)current_node->data;
        current_node_data->key[key_size] = '\n';
        if (!current_node_data->is_key_set || strcmp(current_node_data->key, key))
        {
            (current_node_data->app_cd)(arg, arg_size, _nfc_op, current_node_data->sub_id);
            return SUCCESS;
        }
    }

    return NO_INVOCATION_REQUIRED;
}

notif_chain_t *nfc_create_new_notif_chain(char *notif_chain_name)
{
    notif_chain_t *new = (notif_chain_t *)malloc(sizeof(notif_chain_t));
    if (new == (notif_chain_t *)NULL)
    {
        return NFC_CREATION_FAILURE;
    }

    new->nfc_name = notif_chain_name;
    new->notif_chain = create_doubly_linked_list();
    if (new->notif_chain == (doubly_linked_list_t *)NULL)
    {
        return NFC_CREATION_FAILURE;
    }

    return new;
}

int nfc_delete_all_nfce(notif_chain_t *nfc)
{
    node_t* current_node = nfc->notif_chain->head->next_node;
    node_t* tail = nfc->notif_chain->tail;

    if (current_node = tail)
    {
        return EMPTY_NOTIF_CHAIN;
    }

    while (current_node != tail)
    {
        delete_node (current_node, nfc->notif_chain);
    }

    return SUCCESS;
}
