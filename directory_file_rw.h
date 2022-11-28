#pragma once
#include <stdio.h>
#include <assert.h>

typedef int status_t;

status_t directory_file_read(char* file_name,int* inode_number);
status_t directory_file_write(char* file_name,int inode_number);
int directory_file_exits(char* filename);
int directory_file_exits_inode_no(int inode_number);