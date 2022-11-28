#include "freelist.h"
#include "hashqueue.h"
#include "getblk.h"
#include "hdd.h"
#include "bwrite.h"
#include <time.h>

extern hashqueue_t* p_hashqueue;
extern list_t* p_freelist;
extern FILE* fp_log_file;

void bwrite(buffer_t* buffer)
{
    fprintf(fp_log_file,"------BWRITE------\r\n");
    if(is_IO_sync())
    {
        fprintf(fp_log_file,"Written buffer with block no = %d in memory\r\n",buffer->buffer_header.block_number);
        hdd_in_memory_write(buffer->buffer_header.block_number,buffer->data,BLOCK_SIZE);
    }
    else
    {
        fprintf(fp_log_file,"Buffer is delayed write\r\n");
        buffer->buffer_header.status = DELAYED_WRITE;
        insert_beg(p_freelist,buffer);
    }

    fprintf(fp_log_file,"------BWRITE COMPLETED------\r\n");
    fflush(fp_log_file);
}

int is_IO_sync()
{
    /*srand(time(0));

    if(rand()%2 == 0)
        return TRUE;
    else
        return FALSE;
    */    
    return TRUE;    
}
