#pragma once

#define MAX_IIT_SIZE 32
#define MAX_FT_SIZE 32
#define MAX_UFDT_SIZE 32
#define INVALID_DISK_ADDRESS -3
#define INVALID_INODE_NUMBER -4

typedef struct InCoreInode iit_inode_t;

typedef struct file_table ft_t;
typedef struct user_file_descripter_table ufdt_t;

struct InCoreInode
{
    int file_owner;
    int file_type;
    int file_permissions[9];
    int file_size;
    int disk_addresses[10];

    int inode_status;
    int device_number;
    int inode_number;
    int reference_count;
    //int link_count;

    iit_inode_t* prev_inode_on_hashqueue;
    iit_inode_t* next_inode_on_hashqueue;
    iit_inode_t* prev_inode_on_freelist;
    iit_inode_t* next_inode_on_freelist;
};

struct file_table
{
    int read_offset;
    int write_offset;
    int mode;
    iit_inode_t* iit_pointer;
};

struct user_file_descripter_table
{
    ft_t* ft_pointer;
};

#ifndef INODECACHE_H
#define INODECACHE_H

iit_inode_t* incore_inode_table[MAX_IIT_SIZE];
ft_t* ft[MAX_FT_SIZE];
ufdt_t* ufdt[MAX_UFDT_SIZE];

//int index_in_iit;
//int index_in_ft;
//int index_in_ufdt;

int remembered_inode;
int remembered_disk_block;
#endif

void i_insert_inode_in_iit(iit_inode_t* inode);
void i_show_iit(iit_inode_t* incore_inode_table[MAX_IIT_SIZE],const char* msg);
void i_show_ft(ft_t* ft[MAX_FT_SIZE],const char* msg);
void i_show_ufdt(ufdt_t* ufdt[MAX_UFDT_SIZE],const char* msg);
int check_inode_in_ufdt(iit_inode_t* inode,int* index);
int find_inode_in_iit(int inode_number);
int find_inode_in_ft(int inode_number);
int find_inode_in_ufdt(int inode_number);
int find_free_slot_in_IIT();
int find_free_slot_in_FT();
int find_free_slot_in_UFDT();