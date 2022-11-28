#include "freelist.h"
#include "hashqueue.h"
#include "hdd.h"

extern hashqueue_t* p_hashqueue;
extern list_t* p_freelist;
extern FILE* fp_log_file;

buffer_t* getblk(int device_number,int block_number)
{ 
   buffer_t* hashqueue_buffer = NULL;
   buffer_t* freelist_buffer = NULL;

   status_t s;

   fprintf(fp_log_file,"------IN GETBLK------\r\n");
   fflush(fp_log_file);

   while(1)
   {
       fprintf(fp_log_file,"Searching buffer %d in hashqueue.\r\n",block_number);
       fflush(fp_log_file);

       hashqueue_buffer = search_in_hashqueue(p_hashqueue,block_number);
       
       if(hashqueue_buffer != NULL)
       {
           fprintf(fp_log_file,"Found buffer %d in hashqueue.\r\nMark buffer busy\r\n",hashqueue_buffer->buffer_header.block_number);

           freelist_buffer = search_buffer_in_freelist(p_freelist,block_number);
           if(freelist_buffer == NULL)
           {
               /*s = delete_from_freelist_beg(p_freelist,&freelist_buffer);
               assert(s == SUCCESS && freelist_buffer != NULL);

               freelist_buffer->buffer_header.device_number = 1; 
               freelist_buffer->buffer_header.block_number = block_number;
               freelist_buffer->buffer_header.status = LOCKED;
               freelist_buffer->buffer_header.p_data = freelist_buffer->data; 
               freelist_buffer->buffer_header.next_buffer_on_freelist = NULL;
               freelist_buffer->buffer_header.prev_buffer_on_freelist = NULL;
               freelist_buffer->buffer_header.next_buffer_on_hashqueue = NULL;
               freelist_buffer->buffer_header.prev_buffer_on_hashqueue = NULL;*/
                
           }
           else
           {
               //freelist_buffer->buffer_header.device_number = 1; 
               //freelist_buffer->buffer_header.block_number = block_number;
               //freelist_buffer->buffer_header.status = LOCKED;
               //freelist_buffer->buffer_header.p_data = freelist_buffer->data;
                fprintf(fp_log_file,"Remove buffer %d from freelist\r\n",freelist_buffer->buffer_header.block_number);
                generic_delete_freelist(freelist_buffer);
                //free(freelist_buffer);
                //freelist_buffer = NULL;
           }     
           
            fprintf(fp_log_file,"------GETBLK COMPLETED------\r\n");
            fflush(fp_log_file);

            return hashqueue_buffer;
       }
       else
       {
           fprintf(fp_log_file,"Buffer %d is not hashqueue.\r\n",block_number);

           freelist_buffer = search_buffer_in_freelist(p_freelist,block_number);
           if(freelist_buffer != NULL)
           {
               fprintf(fp_log_file,"Remove buffer %d from freelist\r\n",freelist_buffer->buffer_header.block_number);
               generic_delete_freelist(freelist_buffer);
               if(freelist_buffer->buffer_header.status == DELAYED_WRITE)
                {
                    fprintf(fp_log_file,"Buffer %d marked delayed write\r\n",freelist_buffer->buffer_header.block_number);
                    fprintf(fp_log_file,"Async Write buffer to disk\r\n");
                    hdd_in_memory_write(freelist_buffer->buffer_header.block_number,freelist_buffer->data,strlen(freelist_buffer->data));
                }

                hashqueue_buffer = freelist_buffer;
           }
           else
           {
               hashqueue_buffer = get_list_node(NULL);
           }
        
           fprintf(fp_log_file,"Remove old buffer from hashqueue.\r\nInitialize buffer\r\n");

           delete_from_hashqueue(p_hashqueue,block_number,&hashqueue_buffer);
           hashqueue_buffer->buffer_header.device_number = 1; 
           hashqueue_buffer->buffer_header.block_number = block_number;
           hashqueue_buffer->buffer_header.status = LOCKED ;
           hashqueue_buffer->buffer_header.p_data = hashqueue_buffer->data; 
           hashqueue_buffer->buffer_header.prev_buffer_on_freelist = NULL;
           hashqueue_buffer->buffer_header.next_buffer_on_freelist = NULL;
           hashqueue_buffer->buffer_header.next_buffer_on_hashqueue = NULL;
           hashqueue_buffer->buffer_header.prev_buffer_on_hashqueue = NULL;

        //    if(hashqueue_buffer->data != NULL)
        //    {
        //        free(hashqueue_buffer->data);
        //        hashqueue_buffer->data = NULL;
        //    }

        //    char* hdd_data =  hdd_in_memory_read(block_number);
        //    printf("hdd data = %s",hdd_data);
            
        //    hashqueue_buffer->data = (char*)malloc(strlen(hdd_data));
        //    memcpy(hashqueue_buffer->data,hdd_data,strlen(hdd_data)); 
 
           fprintf(fp_log_file,"Insert new buffer %d in hashqueue.\r\n",hashqueue_buffer->buffer_header.block_number);
           fprintf(fp_log_file,"------GETBLK COMPLETED------\r\n");

           fflush(fp_log_file);

           insert_in_hashqueue(p_hashqueue,hashqueue_buffer); 
           return hashqueue_buffer;
       }
   } 
    return NULL;
}