#include "freelist.h"
#include "hashqueue.h"

extern hashqueue_t* p_hashqueue;
extern list_t* p_freelist;
extern FILE* fp_log_file;

void brelse(buffer_t* buffer)
{
    //wakeup all processes waiting for any buffer to become free
    //wakeup all processes waiting for this buffer to become free
    //raise processor execution level to block interrupts.

    fprintf(fp_log_file,"-----BRELSE-----\r\n");
    if(buffer->buffer_header.status == DELAYED_WRITE)
    {
        fprintf(fp_log_file,"buffer %d is delayed write buffer\r\nInserting at beginning of freelist\r\n",buffer->buffer_header.block_number);
        if(buffer->data != NULL)
        {
            free(buffer->data);
            buffer->data = NULL;
        }
        insert_beg(p_freelist,buffer);
    }   
    else
    {
        fprintf(fp_log_file,"buffer %d is not delayed write buffer\r\nInserting at end of freelist\r\n",buffer->buffer_header.block_number);
        if(buffer->data != NULL)
        {
            free(buffer->data);
            buffer->data = NULL;
        }
        insert_end(p_freelist,buffer);
    }

    fprintf(fp_log_file,"buffer status = unlocked\r\n");

    buffer->buffer_header.status = UNLOCKED;    
    fprintf(fp_log_file,"-----BRELSE COMPLETED-----\r\n");
    fflush(fp_log_file);
}