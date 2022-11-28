#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//To change to 1024*1024 = 1MB
#define HDD_SIZE 1024
#define BLOCK_SIZE 16

#ifndef HDD_H
#define HDD_H

char* hdd[1024];

#endif

void hdd_initialize();
void hdd_in_memory_write(int block_number, char* data,int no_of_bytes);
char* hdd_in_memory_read(int block_number);
void hdd_disk_write(char* hdd[HDD_SIZE]);
void hdd_disk_read(char* hdd[HDD_SIZE]); 
void destroy_in_memory_hdd();
