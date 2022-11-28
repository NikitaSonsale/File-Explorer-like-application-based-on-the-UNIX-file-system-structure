#include "freelist_inode.h"
#include "hashqueue_inode.h"
#include "iput.h"
#include "directory_file_rw.h"
#include <stdlib.h>
#include <Windows.h>

extern ufdt_t* ufdt[MAX_UFDT_SIZE];
extern ft_t* ft[MAX_FT_SIZE];
extern i_hashqueue_t* i_p_hashqueue;
extern i_list_t* i_p_freelist;
extern iit_inode_t* incore_inode_table[MAX_IIT_SIZE];
extern FILE* fp_log_file;

status_t close(int fd)
{
    fprintf(fp_log_file,"------CLOSE SYSTEM CALL------\r\n");

    ufdt_t* ufdt_entry = ufdt[fd];

    //int index_in_ft = find_inode_in_ft(ufdt[fd]->ft_pointer->iit_pointer->inode_number);
    
    //ufdt[fd]->ft_pointer->iit_pointer->link_count = ufdt[fd]->ft_pointer->iit_pointer->link_count-1;

    //int index_in_iit = find_inode_in_iit(ufdt[fd]->ft_pointer->iit_pointer->inode_number);

    //incore_inode_table[index_in_iit]->link_count = incore_inode_table[index_in_iit]->link_count - 1; 

    fprintf(fp_log_file,"iput the inode\r\n");

    iput(ufdt[fd]->ft_pointer->iit_pointer);

    fprintf(fp_log_file,"free file table entry\r\n");
    fflush(fp_log_file);

    if(ufdt[fd] != NULL && ufdt[fd]->ft_pointer != NULL)
    {
        free(ufdt[fd]->ft_pointer);
        ufdt[fd]->ft_pointer = NULL;
    }
    
    /*if(ft[index_in_ft] != NULL)
    {
        free(ft[index_in_ft]);
        ft[index_in_ft] = NULL;
        ufdt[fd]->ft_pointer = NULL;
    }*/

    fprintf(fp_log_file,"free user file descriptor entry\r\n");
    fflush(fp_log_file);

    if(ufdt[fd] != NULL)
    {
        free(ufdt[fd]);
        ufdt[fd] = NULL;
    }
    
    fprintf(fp_log_file,"------CLOSE SYSTEM CALL COMPLETED------\r\n");
    fflush(fp_log_file);

    return SUCCESS;
}

int getFdFromFileName(char* file_name)
{
    int inode_number;

    int s = directory_file_read(file_name,&inode_number);

    if(s == -1)
    {
        return -2;
    }

    int fd = find_inode_in_ufdt(inode_number);

    return fd;
}