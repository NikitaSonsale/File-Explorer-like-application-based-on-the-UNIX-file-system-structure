#include "ialloc.h"
#include "freelist_inode.h"
#include "hashqueue_inode.h"
#include "iget.h"
#include <Windows.h>

extern super_block_t* super_block;
extern int remembered_inode;
extern int remembered_disk_block;
extern int remembered_file_owner_number;
extern i_hashqueue_t* i_p_hashqueue ;
extern int file_permissions[9];
extern FILE* fp_log_file;

void ialloc(iit_inode_t** inode)
{
    int i,j;

    fprintf(fp_log_file,"------IN IALLOC------\r\n");

    remembered_inode = super_block->remembered_inode;
    remembered_disk_block = super_block->remembered_datablock;

    if(super_block->index_to_next_free_inode >= SUPER_BLOCK_LIST_SIZE)
    {
        fprintf(fp_log_file,"Superblock index_to_next_free_inode = %d \r\nFilling up the superblock!!\r\n",super_block->index_to_next_free_inode);
        super_block->index_to_next_free_inode = 0;
        super_block->size_of_inode_list = 10;
        remembered_inode = remembered_inode+10;
        for(i=0,j=0;j<10;i++,j++)
        {
            super_block->list_of_free_inodes[j] = i+remembered_inode;
        }

        fprintf(fp_log_file,"New remembered inode = %d \r\n",remembered_inode);

        super_block->remembered_inode = remembered_inode;
    } 

    super_block->index_to_next_free_datablock = 0;
    super_block->size_of_data_block = 0;
    for(i=0,j=0;j<10;i++,j++)
    {
        super_block->list_of_free_datablocks[j] = i+remembered_disk_block;
    }   

    remembered_disk_block = remembered_disk_block+10; 

    super_block->remembered_datablock = remembered_disk_block;

    int inode_number = super_block->list_of_free_inodes[super_block->index_to_next_free_inode];

    super_block->list_of_free_inodes[super_block->index_to_next_free_inode] = INVALID_INODE_NUMBER;

    super_block->index_to_next_free_inode++;
    super_block->index_to_next_free_datablock = 0;
    super_block->size_of_inode_list--;

    iit_inode_t* i_get_inode = iget(inode_number);
    assert(i_get_inode != NULL);

    i_get_inode->device_number = 1;
    //i_get_inode->link_count = 1;
    for(i=0 ;i <10;i++)
    {
        i_get_inode->disk_addresses[i] = super_block->list_of_free_datablocks[i];
    }

    for(i=0 ;i <9;i++)
    {
        i_get_inode->file_permissions[i] = file_permissions[i];
    }
    
    if(check_if_file_owner_exits("tanvi") == TRUE)
    {
        i_get_inode->file_owner = get_file_owner_number("tanvi");
    }
    else
    {
        write_file_owner("misc",remembered_file_owner_number);
        remembered_file_owner_number++;
    }
    
    i_get_inode->file_size = 0;
    i_get_inode->file_type = REGULAR_FILE;
    i_get_inode->inode_number = inode_number;
    i_get_inode->inode_status = LOCKED;
    i_get_inode->reference_count = 1;

    dilb_in_memory_write(inode_number,i_get_inode->file_owner,i_get_inode->file_size,i_get_inode->file_type,i_get_inode->file_permissions,i_get_inode->disk_addresses);

    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        super_block->list_of_free_datablocks[i] = INVALID_DISK_ADDRESS;
    }
    
    *inode =  i_get_inode;
    fprintf(fp_log_file,"------IALLOC COMPLETED------\r\n");
    fflush(fp_log_file);
}