#pragma once

#ifndef HDD_H
#define HDD_H

#include "hdd.h"

#endif

#ifndef DILB_H
#define DILB_H

#include "dilb.h"

#endif

#define SUPER_BLOCK_LIST_SIZE 10

typedef struct Super_block super_block_t;

struct Super_block
{
    int size_of_inode_list;
    int index_to_next_free_inode;
    int list_of_free_inodes[SUPER_BLOCK_LIST_SIZE];

    int size_of_data_block;
    int index_to_next_free_datablock;
    int list_of_free_datablocks[SUPER_BLOCK_LIST_SIZE];

    int remembered_inode;
    int remembered_datablock;
}; 

#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

super_block_t* super_block;

#endif

void superblk_initialize();
void superblk_in_memory_write(int sizex_of_inode_list, int index_to_next_free_inode, int* list_of_free_inodes,int size_of_db_list, int index_to_next_free_db, int* list_of_free_db,int remembered_inode,int remembered_datablock);
void superblk_disk_write(super_block_t* superblk);
void superblk_disk_read(super_block_t* superblk);
void print_superblk(super_block_t* super_blk);
void destroy_superblk(super_block_t* superblk);