#include "open.h"
#include "directory_file_rw.h"
#include "iget.h"
#include "iput.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <windows.h>

extern ufdt_t* ufdt[MAX_UFDT_SIZE];
extern ft_t* ft[MAX_FT_SIZE];
extern FILE* fp_log_file;

file_descriptor_t open(char* file_name,int mode)
{
    fprintf(fp_log_file,"\r\n------IN OPEN SYSTEM CALL------\r\n");

    int inode_number=-1;
    iit_inode_t* iit_inode = NULL;
    status_t s;

    //Convert filename into inode_number
    s = directory_file_read(file_name,&inode_number);
    //printf("inode no = %d\n",inode_number);

    if(s != 0)
        return FILE_DOES_NOT_EXISTS;

    /*if(inode_number == -1 )
        return FILE_DOES_NOT_EXISTS;*/

    //get that inode in IIT
    iit_inode = iget(inode_number);
    assert(iit_inode != NULL);

    if(mode == READ)
    {
        if(iit_inode->file_permissions[0] != 1)
        {   
            iput(iit_inode);
            return FILE_PERMISSIONS_NOT_VALID;
        }
    }
    else if(mode == WRITE)
    {
        if(iit_inode->file_permissions[1] != 1)
        {   
            iput(iit_inode);
            return FILE_PERMISSIONS_NOT_VALID;
        }
    }

    fprintf(fp_log_file,"File %s exits and access permitted.\r\n",file_name);

    //iit_inode->link_count = iit_inode->link_count+1;
    iit_inode->file_type = REGULAR_FILE;

    i_insert_inode_in_iit(iit_inode);

    int index_in_ft = find_free_slot_in_FT();

    fprintf(fp_log_file,"Inserted inode %d at position %d in File Table\r\n",iit_inode->inode_number,index_in_ft);

    ft[index_in_ft] = (ft_t*)malloc(sizeof(ft_t));
    assert(ft[index_in_ft] != NULL);

    ft[index_in_ft]->iit_pointer = iit_inode;
    ft[index_in_ft]->mode = mode;
    ft[index_in_ft]->read_offset = 0;
    ft[index_in_ft]->write_offset = 0;

    int index_in_ufdt = find_free_slot_in_UFDT();

    ufdt[index_in_ufdt] = (ufdt_t*)malloc(sizeof(ufdt_t));
    assert(ufdt[index_in_ufdt] != NULL);

    ufdt[index_in_ufdt]->ft_pointer = ft[index_in_ft];

    fprintf(fp_log_file,"Inserted inode %d at position %d in User File Descriptor Table\r\n",iit_inode->inode_number,index_in_ufdt);

    fprintf(fp_log_file,"------OPEN SYSTEM CALL COMPLETED------\r\n");
    fflush(fp_log_file);

    return index_in_ufdt;
}