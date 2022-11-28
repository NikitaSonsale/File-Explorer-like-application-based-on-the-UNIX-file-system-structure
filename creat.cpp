#include "creat.h"
#include "ialloc.h"
#include "directory_file_rw.h"
#include "hashqueue_inode.h"
#include "iget.h"
#include "iput.h"
#include "alloc.h"
#include "free.h"
#include <assert.h>

extern int remembered_disk_block;
extern i_hashqueue_t* i_p_hashqueue;
extern ufdt_t* ufdt[MAX_UFDT_SIZE];
extern ft_t* ft[MAX_FT_SIZE];
extern iit_inode_t* incore_inode_table[MAX_IIT_SIZE];
extern int file_creation_mode;
extern int file_permissions[9];
extern FILE* fp_log_file;

/*For file creation mode can be
READ
WRITE*/

int creat(char* file_name,int mode)
{
    int index_in_ft,index_in_ufdt;
    iit_inode_t* inode;
    int inode_number;
    int error_flag=0;
    int i;
    buffer_t* buffer=NULL;

    fprintf(fp_log_file,"\r\n------IN CREAT SYSTEM CALL------\r\n");
    fflush(fp_log_file);
    
    file_creation_mode = mode;

    //get inode for file name -> namei = 
    if(directory_file_exits(file_name) == 1)
    {
        fprintf(fp_log_file,"File %s exits in this directory\r\n",file_name);

        directory_file_read(file_name,&inode_number);

        inode = iget(inode_number);
        assert(inode != NULL);
        
        /*for(i=0;i<10;i++)
        {
            printf("%d  ",inode->file_permissions[i]);
        }*/

        /*if(mode == READ)
        {
            if(inode->file_permissions[0] != 1)
                error_flag=1;
        }
        else if(mode == WRITE)
        {
            if(inode->file_permissions[1] != 1)
                error_flag=1;
        }
        else if(mode = RDWR)
        {
            if(inode->file_permissions[0] != 1 || inode->file_permissions[1] != 1 )
                error_flag=1;
        }

        if(error_flag == 1)
        {
            iput(inode);
            return PERMISSION_ERROR;
        }*/

        if(inode->file_permissions[1] != 1)
        {   
            fprintf(fp_log_file,"File does not have write permissions\r\n");
            fflush(fp_log_file);

            iput(inode);
            return PERMISSION_ERROR;
        }

        //Truncate already existing data in file
        fprintf(fp_log_file,"Truncating old file contents\r\n");
        fflush(fp_log_file);

        for(i=0;i<10;i++)
        {
            //printf("Freeling block = %d\n",inode->disk_addresses[i]);
            block_free(inode->disk_addresses[i]);
            inode->disk_addresses[i]= INVALID_DISK_ADDRESS;
        }

        dilb_in_memory_write(inode->inode_number,inode->file_owner,inode->file_size,inode->file_type,inode->file_permissions,inode->disk_addresses);

        fprintf(fp_log_file,"Allocating new file contents\r\n");

        for(i=0;i<10;i++)
        {
            alloc(&buffer);
            assert(buffer != NULL);

            inode->disk_addresses[i] = buffer->buffer_header.block_number;
        }

        for(i=0;i<9;i++)
        {
            inode->file_permissions[i] = 0;
        }

        for(i=0;i<9;i++)
        {
            inode->file_permissions[i] = file_permissions[i];
        }

        //inode->link_count = 1;

        dilb_in_memory_write(inode->inode_number,inode->file_owner,inode->file_size,inode->file_type,inode->file_permissions,inode->disk_addresses);
    }
    else
    {
        fprintf(fp_log_file,"File %s does not exits in this directory\r\nCreating a new file.\r\n",file_name);
        
        ialloc(&inode);
        assert(inode != NULL);

        directory_file_write(file_name,inode->inode_number);
    }
    
    /*if(directory_file_exits(file_name) == 0)
    {
        directory_file_write(file_name,inode->inode_number);
    }*/

    i_insert_inode_in_iit(inode);

    index_in_ft = find_free_slot_in_FT();

    ft[index_in_ft] = (ft_t*)malloc(sizeof(ft_t));
    assert(ft[index_in_ft] != NULL);

    //printf("index_in_iit = %d",index_in_iit);
    ft[index_in_ft]->iit_pointer = inode;
 
    ft[index_in_ft]->mode = file_creation_mode;
    ft[index_in_ft]->read_offset = 0;
    ft[index_in_ft]->write_offset = 0;

    fprintf(fp_log_file,"Inserted inode %d at position %d in File Table\r\n",inode->inode_number,index_in_ft);

    index_in_ufdt = find_free_slot_in_UFDT();
    ufdt[index_in_ufdt] = (ufdt_t*)malloc(sizeof(ufdt_t));
    assert(ufdt[index_in_ufdt] != NULL);

    ufdt[index_in_ufdt]->ft_pointer = ft[index_in_ft];

    fprintf(fp_log_file,"Inserted inode %d at position %d in User File Descriptor Table\r\n",inode->inode_number,index_in_ufdt);

    //int file_desc = index_in_ufdt;

    //printf("\nAllocated disk addresses : \n");
    /*for(i=0;i<10;i++)
    {
        printf("%d  ",inode->disk_addresses[i]);
    }*/

    fprintf(fp_log_file,"------CREAT SYSTEM CALL COMPLETED------\r\n");
    fflush(fp_log_file);

    return index_in_ufdt;
}