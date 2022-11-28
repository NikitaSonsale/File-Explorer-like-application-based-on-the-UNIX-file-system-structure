#pragma once

#ifndef INODECACHE_H
#define INODECACHE_H

#include "inodecache.h"

#endif

#include "freelist_inode.h"

#define INODE_NOT_FOUND -1

//list_t* [4] 
typedef i_list_t* i_hashqueueHeaders_t[4];
typedef i_hashqueueHeaders_t i_hashqueue_t;


i_hashqueue_t* i_create_hashqueue();
void i_show_hashqueue(i_hashqueue_t* p_hashqueue,const char* msg);
status_t i_insert_in_hashqueue(i_hashqueue_t* p_hashqueue, iit_inode_t* insert_inode);
void i_generic_insert_hashqueue(i_node_t *p_beg,i_node_t* p_mid, i_node_t* p_end);
iit_inode_t* i_search_in_hashqueue(i_hashqueue_t* p_hashqueue,int inode_number);
void i_generic_delete_hashqueue(iit_inode_t* p_delete_node);
status_t i_delete_from_hashqueue(i_hashqueue_t* p_hashqueue,int inode_number,iit_inode_t** pp_inode);
status_t i_destroy_hashqueue(i_hashqueue_t** pp_hashqueue);