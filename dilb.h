#pragma once

#define MAX_DILB_SIZE 1024

typedef struct DILBInode dilb_inode_t;

struct DILBInode
{
    int inode_number;
    int file_owner;
    int file_type;
    int file_permissions[9];
    int file_size;
    int disk_addresses[10];
};

#ifndef DILB_H
#define DILB_H

dilb_inode_t* dilb[MAX_DILB_SIZE];

#endif

void dilb_initialize();
void dilb_in_memory_write(int inode_number, int file_owner, int file_size, int file_type, int* file_permissions,int* disk_addr);
dilb_inode_t* dilb_in_memory_read(int inode_number);
void dilb_disk_write(dilb_inode_t* dilb[MAX_DILB_SIZE]);
void dilb_disk_read(dilb_inode_t* dilb[MAX_DILB_SIZE]);
void destroy_dilb(dilb_inode_t* dilb[MAX_DILB_SIZE]);