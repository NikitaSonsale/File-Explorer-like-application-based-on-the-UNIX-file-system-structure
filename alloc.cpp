#include "freelist_inode.h"
#include "hashqueue_inode.h"
#include "getblk.h"
#include "alloc.h"

#include <assert.h>

extern super_block_t* super_block;
extern int remembered_inode;
extern int remembered_disk_block;
extern FILE* fp_log_file;

void alloc(buffer_t** block)
{
    fprintf(fp_log_file,"------IN ALLOC------\r\n");

    int i,j;
    buffer_t* buffer = NULL;
    int block_number;

    remembered_disk_block = super_block->remembered_datablock;

    if(super_block->index_to_next_free_datablock >= SUPER_BLOCK_LIST_SIZE)
    {
        super_block->index_to_next_free_datablock = 0;
        super_block->size_of_data_block = 10;
        for(i=0,j=0;j<10;i++,j++)
        {
            super_block->list_of_free_datablocks[j] = i+remembered_disk_block;
        }

        remembered_disk_block = remembered_disk_block+10;
        super_block->remembered_datablock = remembered_disk_block;
    } 

    block_number = super_block->list_of_free_datablocks[super_block->index_to_next_free_datablock];

    buffer = getblk(1,block_number);
    assert(buffer != NULL);

    super_block->index_to_next_free_datablock++;
    super_block->size_of_data_block--;

    *block = buffer;
    fprintf(fp_log_file,"Allocated block = %d\r\n",buffer->buffer_header.block_number);

    fprintf(fp_log_file,"------ALLOC COMPLETED------\r\n");
    fflush(fp_log_file);
}