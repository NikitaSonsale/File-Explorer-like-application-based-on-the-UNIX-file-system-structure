#include "brelse.h"
#include "getblk.h"
#include "free.h"
#include "freelist.h"
#include "hashqueue.h"

extern super_block_t* super_block;
extern hashqueue_t* p_hashqueue;
extern FILE* fp_log_file;

void block_free(int block_number)
{
    fprintf(fp_log_file,"-----FREE-----\r\n");

    fprintf(fp_log_file,"Superblock's size of free data blocks + 1\r\n");
    super_block->size_of_data_block++;

    int i;
    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        if(super_block->list_of_free_datablocks[i] == INVALID_DISK_ADDRESS)
        {
            super_block->list_of_free_datablocks[i] = block_number;
            break;
        }
    }

    //show_hashqueue(p_hashqueue,"\nhashqueue:\n");
    buffer_t* buffer = getblk(1,block_number);

    brelse(buffer);

    fprintf(fp_log_file,"-----FREE COMPLETED-----\r\n");
    fflush(fp_log_file);

    return;
}