#pragma once 

#ifndef INODECACHE_H
#define INODECACHE_H

#include "inodecache.h"

#endif

#include "freelist.h"

typedef iit_inode_t i_node_t;
typedef i_node_t i_list_t;


enum inode_status
{
    I_UNLOCKED=1,
    I_LOCKED=2
};

i_list_t *i_create_list();
i_node_t* i_get_list_node();
void* i_xmalloc(int size);
status_t i_insert_end(i_list_t* p_list,iit_inode_t* insert_inode);
void i_generic_insert(i_node_t *p_beg,i_node_t* p_mid, i_node_t* p_end);
status_t i_show_freelist(i_list_t* p_list,const char* msg);
len_t i_get_freelist_length(i_list_t* p_list);
int i_is_freelist_empty(i_list_t* p_list);
void i_generic_delete_freelist(i_node_t* p_delete_node);
status_t i_delete_from_freelist_beg(i_list_t* p_list,iit_inode_t** pp_inode);
status_t i_insert_beg(i_list_t* p_list,iit_inode_t* insert_inode);
status_t i_initialize_freelist(i_list_t* p_freelist);
iit_inode_t* i_search_inode_in_freelist(i_list_t* p_freelist,int inode_number);
status_t i_destroy_freelist(i_list_t** pp_freelist);