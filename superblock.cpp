#include "superblock.h"
#include <stdio.h>
#include <assert.h>

extern dilb_inode_t* dilb[MAX_DILB_SIZE];
extern super_block_t* super_block;

void superblk_initialize()
{
    super_block = (super_block_t*)malloc(sizeof(super_block_t));
    assert(super_block!=NULL);

    super_block->size_of_inode_list = SUPER_BLOCK_LIST_SIZE; 
    super_block->index_to_next_free_inode = 0;

    int i;

    super_block->size_of_data_block = SUPER_BLOCK_LIST_SIZE;
    super_block->index_to_next_free_datablock = 0;
    
    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        super_block->list_of_free_inodes[i] = i;
        super_block->list_of_free_datablocks[i] = i;
    }

    super_block->remembered_inode = 0;
    super_block->remembered_datablock = 0;
}

void superblk_in_memory_write(int sizex_of_inode_list, int index_to_next_free_inode, int* list_of_free_inodes,int size_of_db_list, int index_to_next_free_db, int* list_of_free_db,int remembered_inode,int remembered_datablock)
{
    super_block->size_of_inode_list = sizex_of_inode_list;
    super_block->index_to_next_free_inode = index_to_next_free_inode;
    super_block->size_of_data_block = size_of_db_list;
    super_block->index_to_next_free_datablock = index_to_next_free_db;
    
    int i;
    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        super_block->list_of_free_inodes[i] = list_of_free_inodes[i];
        super_block->list_of_free_datablocks[i] = list_of_free_db[i];
    }

    super_block->remembered_inode = remembered_inode;
    super_block->remembered_datablock = remembered_datablock;
}

void print_superblk(super_block_t* super_blk)
{
    printf("\nSize of inode list = %d\n",super_blk->size_of_inode_list);
    printf("\nIndex of next inode in list = %d\n",super_blk->index_to_next_free_inode);

    printf("\nArray of free inodes\n");
    int i;
    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        printf("%d ",super_blk->list_of_free_inodes[i]);
    }

    printf("\nSize of data block list = %d\n",super_blk->size_of_data_block);
    printf("\nIndex of next data block in list = %d\n",super_blk->index_to_next_free_datablock);

    printf("\nArray of free data blocks\n");
    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        printf("%d ",super_blk->list_of_free_datablocks[i] );
    }

    printf("\nRemembered Inode = %d\n",super_blk->remembered_inode);
    printf("Remembered Datablock = %d\n",super_blk->remembered_datablock);
}

void destroy_superblk(super_block_t* superblk)
{
    if(superblk != NULL)
    {
        free(superblk);
        superblk = NULL;
    }
}
  
void superblk_disk_write(super_block_t* super_block)
{
    FILE* fd_superblk_file = fopen("superblock.txt","w");
    if(fd_superblk_file == NULL)
    {
     printf("Error:Cannot open data file\n");
     exit(1);
    }    

    fprintf(fd_superblk_file,"%d %d %d %d ",super_block->size_of_inode_list,super_block->index_to_next_free_inode,super_block->size_of_data_block,super_block->index_to_next_free_datablock);
    int i;
    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        fprintf(fd_superblk_file,"%d ",super_block->list_of_free_inodes[i]);
    }

    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        fprintf(fd_superblk_file,"%d ",super_block->list_of_free_datablocks[i]);
    }

    fprintf(fd_superblk_file,"%d %d",super_block->remembered_inode,super_block->remembered_datablock);

    fflush(fd_superblk_file);
    fclose(fd_superblk_file);
    fd_superblk_file = NULL;
}

void superblk_disk_read(super_block_t* super_block)
{
    FILE* fd_superblk_file = fopen("superblock.txt","r");
    if(fd_superblk_file == NULL)
    {
     printf("Error:Cannot open data file\n");
     exit(1);
    }

    fscanf(fd_superblk_file, "%d %d %d %d",&super_block->size_of_inode_list,&super_block->index_to_next_free_inode,&super_block->size_of_data_block,&super_block->index_to_next_free_datablock);    

    int i;

    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        fscanf(fd_superblk_file,"%d ",&super_block->list_of_free_inodes[i]);
    }

    for(i=0;i<SUPER_BLOCK_LIST_SIZE;i++)
    {
        fscanf(fd_superblk_file,"%d ",&super_block->list_of_free_datablocks[i]);
    }

    fscanf(fd_superblk_file,"%d %d",&super_block->remembered_inode,&super_block->remembered_datablock);

    fclose(fd_superblk_file);
    fd_superblk_file = NULL;
}