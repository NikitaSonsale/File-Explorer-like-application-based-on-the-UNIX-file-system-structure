#pragma once 

#include "buffercache.h"

#ifndef HDD_H
#define HDD_H

#include "hdd.h"

#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILURE 0
#define LIST_DATA_NOT_FOUND 2
#define LIST_EMPTY 3
#define INVALID_BLOCK_NUMBER -1

#define BUFFERDATASIZE 16
#define FREELISTSIZE 32

#ifndef buffer_t
    typedef struct Buffer buffer_t;
#endif

typedef char* data_t;
typedef int status_t;
typedef int len_t;
//typedef int bool;
typedef buffer_t node_t;
typedef node_t list_t;

enum buffer_status
{
    UNLOCKED=1,
    LOCKED=2,
    DELAYED_WRITE=4,
    VALID=8
};

list_t *create_list();
status_t initialize_freelist(list_t* p_freelist);
status_t insert_end(list_t* p_list,buffer_t* insert_buffer);
status_t insert_beg(list_t* p_list,buffer_t* insert_buffer);

status_t delete_from_freelist_beg(list_t* p_list,buffer_t** pp_buffer);
buffer_t* search_buffer_in_freelist(list_t* p_freelist,int block_number);

status_t show_freelist(list_t* p_list,const char* msg);

len_t get_freelist_length(list_t* p_list);
bool is_freelist_empty(list_t* p_list);

void generic_insert(node_t *p_beg,node_t* p_mid, node_t* p_end);
void generic_delete_freelist(node_t* p_delete_node);

void destroy_freelist(list_t* p_list);

node_t* get_list_node(data_t data);
void* xmalloc(int size);
