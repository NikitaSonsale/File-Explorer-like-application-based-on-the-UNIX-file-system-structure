#include "ifree.h"

extern super_block_t* super_block;
extern FILE* fp_log_file;

void ifree(int inode_number)
{
    fprintf(fp_log_file,"------IN IFREE------\r\n");
    fprintf(fp_log_file,"Superblock's size of free inodes + 1\r\nAdjust remembered inode.\r\n");
    super_block->size_of_inode_list++;

    int i;
    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        if(super_block->list_of_free_inodes[i] == INVALID_INODE_NUMBER)
        {
            super_block->list_of_free_inodes[i] = inode_number;
            break;
        }
    }

    /*Here there should be code to change the remembered inode*/
    fprintf(fp_log_file,"------IFREE COMPLETED------\r\n");
    fflush(fp_log_file);
}