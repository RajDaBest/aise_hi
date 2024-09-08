#ifndef _NOTIF_H
#define _NOTIF_H

#include "doubly_linked_list.h"
#include <stddef.h>
#include <stdbool.h>

#define MAX_NOTIF_KEY_SIZE 128
#define EMPTY_NOTIF_CHAIN 1
#define NO_INVOCATION_REQUIRED 2
#define NFC_CREATION_FAILURE NULL

typedef enum
{
    NFC_UNKNOWN,
    NFC_ADD,
    NFC_MOD,
    NFC_DEL,
} nfc_op_t;

typedef struct
{
    char* nfc_name;
    doubly_linked_list_t *notif_chain;
} notif_chain_t;

typedef void (*nfc_app_cb)(void *, size_t, nfc_op_t*, int);

typedef struct
{
    int sub_id;
    char key[MAX_NOTIF_KEY_SIZE];
    size_t key_size;
    bool is_key_set;
    nfc_app_cb app_cd;
} notif_chain_element_data_t;

notif_chain_t *nfc_create_new_notif_chain(char *notif_chain_name);
int nfc_delete_all_nfce (notif_chain_t* nfc);
int nfc_register_notif_chain(notif_chain_t *nfc, notif_chain_element_data_t *nfce);
int nfc_invoke_notif_chain(notif_chain_t *nfc, void *arg, size_t arg_size, char *key, size_t key_size, nfc_op_t* _nfc_op);

#endif