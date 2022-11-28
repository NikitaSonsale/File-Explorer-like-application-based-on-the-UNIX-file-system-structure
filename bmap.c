#include "bmap.h"
#include "freelist.h"

void bmap(iit_inode_t* inode ,int byte_offset,int* array_index,int* array_offset)
{
    *array_index = byte_offset / BUFFERDATASIZE; 
    *array_offset = byte_offset % BUFFERDATASIZE;
}