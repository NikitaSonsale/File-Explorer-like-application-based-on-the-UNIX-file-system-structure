#pragma once

#include "buffercache.h"

#define BUFFER_NOT_FOUND -1

//list_t* [4] 
typedef list_t* hashqueueHeaders_t[4];
typedef hashqueueHeaders_t hashqueue_t;

hashqueue_t* create_hashqueue();
void show_hashqueue(hashqueue_t* p_hashqueue,const char* msg);
status_t insert_in_hashqueue(hashqueue_t* p_hashqueue, buffer_t* insert_buffer);
void generic_insert_hashqueue(node_t *p_beg,node_t* p_mid, node_t* p_end);
buffer_t* search_in_hashqueue(hashqueue_t* p_hashqueue,int blockNumber);
void generic_delete_hashqueue(buffer_t* p_delete_node);
status_t delete_from_hashqueue(hashqueue_t* p_hashqueue,int block_number,buffer_t** pp_buffer);
void destroy_hashqueue(hashqueue_t* p_hashqueue);