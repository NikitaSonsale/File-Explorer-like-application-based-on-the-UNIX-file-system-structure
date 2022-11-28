#include "freelist.h"
#include "hashqueue.h"
#include "getblk.h"
#include "hdd.h"

extern hashqueue_t* p_hashqueue;
extern list_t* p_freelist;
extern FILE* fp_log_file;

void bread(int block_number,buffer_t** pp_buffer)
{
    fprintf(fp_log_file,"------BREAD------\r\n");
    buffer_t* read_buffer = getblk(1,block_number);

    if(read_buffer->buffer_header.status == (LOCKED&VALID))
    {
        *pp_buffer = read_buffer;
        return ;
    }
    
    //Ha kay chapripana kelay!!!
    //read_buffer->data =  hdd_in_memory_read(block_number);
    read_buffer->buffer_header.status = (LOCKED&VALID);

    if(read_buffer->data == NULL)
    {
        read_buffer->data = (char*)calloc(BLOCK_SIZE,sizeof(char));
    }

    strncpy(read_buffer->data,hdd_in_memory_read(block_number),16);
    //read_buffer->data[BLOCK_SIZE]='\0';

    *pp_buffer = read_buffer;   
    fprintf(fp_log_file,"------BREAD COMPLETED------\r\n");
    fflush(fp_log_file);
}