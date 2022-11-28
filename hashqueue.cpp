#include "freelist.h"
#include "hashqueue.h"

hashqueue_t* p_hashqueue = NULL;
extern list_t* p_freelist;

hashqueue_t* create_hashqueue(void)
{
    hashqueue_t* p_hashqueue = (hashqueue_t*)malloc(sizeof(hashqueue_t));

    int i;
    for(i=0;i<4;i++)
    {
        (*(p_hashqueue))[i] = get_list_node(NULL);
        (*(p_hashqueue))[i]->buffer_header.device_number = 1;
        (*(p_hashqueue))[i]->buffer_header.block_number = INVALID_BLOCK_NUMBER;
        (*(p_hashqueue))[i]->buffer_header.status = UNLOCKED;
        (*(p_hashqueue))[i]->buffer_header.p_data = (*(p_hashqueue))[i]->data;
        (*(p_hashqueue))[i]->buffer_header.prev_buffer_on_hashqueue = (*(p_hashqueue))[i];
        (*(p_hashqueue))[i]->buffer_header.next_buffer_on_hashqueue = (*(p_hashqueue))[i];
        (*(p_hashqueue))[i]->buffer_header.next_buffer_on_freelist = NULL;
        (*(p_hashqueue))[i]->buffer_header.prev_buffer_on_freelist = NULL;
    }    

    return p_hashqueue;
}

status_t insert_in_hashqueue(hashqueue_t* p_hashqueue, buffer_t* insert_buffer)
{
    list_t* p_list ;
    int block_number = insert_buffer->buffer_header.block_number;

    ///block no -> 10 block_number%4 = 2 (*(p_hashqueue))[2]
    p_list = (*(p_hashqueue))[(block_number%4)];
    
    //Here new node must not be created. Node from freelist has to be inserted
    generic_insert_hashqueue(p_list->buffer_header.prev_buffer_on_hashqueue,insert_buffer,p_list);

    return SUCCESS;
}

void generic_insert_hashqueue(node_t *p_beg,node_t* p_mid, node_t* p_end)
{
    p_mid->buffer_header.next_buffer_on_hashqueue = p_end;
    p_mid->buffer_header.prev_buffer_on_hashqueue = p_beg;
    p_beg->buffer_header.next_buffer_on_hashqueue = p_mid;
    p_end->buffer_header.prev_buffer_on_hashqueue = p_mid;
}

buffer_t* search_in_hashqueue(hashqueue_t* p_hashqueue,int block_number)
{
    node_t *p_run;
    list_t* p_list ;
    p_list = (*(p_hashqueue))[(block_number%4)];

   // printf("%d %d",p_list->buffer_header.block_number,block_number);
    
    //printf("Returning %s",p_buffer->data);
    for(p_run = p_list->buffer_header.next_buffer_on_hashqueue; p_run != p_list; p_run = p_run->buffer_header.next_buffer_on_hashqueue)
    {
        if(p_run->buffer_header.block_number == block_number)
        {
            return p_run;
        }
    }
    return NULL;
}

status_t delete_from_hashqueue(hashqueue_t* p_hashqueue,int block_number,buffer_t** pp_buffer)
{
    buffer_t* buffer_to_delete =  search_in_hashqueue(p_hashqueue,block_number);

    if(buffer_to_delete == NULL)
    {
        return BUFFER_NOT_FOUND;
    }
    else
    {
        // buffer_t* buffer = get_list_node(buffer_to_delete->data);
        // buffer->buffer_header.block_number = buffer_to_delete->buffer_header.block_number;
        // buffer->buffer_header.device_number = buffer_to_delete->buffer_header.device_number;
        // buffer->buffer_header.status = buffer_to_delete->buffer_header.status;
        // buffer->buffer_header.next_buffer_on_freelist = buffer_to_delete->buffer_header.next_buffer_on_freelist;
        // buffer->buffer_header.prev_buffer_on_freelist = buffer_to_delete->buffer_header.prev_buffer_on_freelist;
        // buffer->buffer_header.next_buffer_on_hashqueue = NULL;
        // buffer->buffer_header.prev_buffer_on_hashqueue = NULL;

        *pp_buffer = buffer_to_delete;
        
        generic_delete_hashqueue(buffer_to_delete);  
        return SUCCESS; 
    }      
}

void generic_delete_hashqueue(buffer_t* p_delete_node)
{
    p_delete_node->buffer_header.next_buffer_on_hashqueue->buffer_header.prev_buffer_on_hashqueue = p_delete_node->buffer_header.prev_buffer_on_hashqueue;
    p_delete_node->buffer_header.prev_buffer_on_hashqueue->buffer_header.next_buffer_on_hashqueue = p_delete_node->buffer_header.next_buffer_on_hashqueue;

}

void show_hashqueue(hashqueue_t* p_hashqueue,const char* msg)
{
    node_t *p_run;

    if(msg)
    {
        puts(msg);
    }

    int i;
    hashqueueHeaders_t hashqueue ;
    
    for(i=0;i<4;i++)
    {
        hashqueue[i] = (*(p_hashqueue))[i];
        printf("[Beg]<->");
        for(p_run = hashqueue[i]->buffer_header.next_buffer_on_hashqueue; p_run != hashqueue[i]; p_run = p_run->buffer_header.next_buffer_on_hashqueue)
        {
            printf("%d:%s <->",p_run->buffer_header.block_number, p_run->data);
        }
        puts("[End]");
    }
}

void destroy_hashqueue(hashqueue_t* p_hashqueue)
{
    int i;
    buffer_t* temp = NULL;
    buffer_t* next = NULL;
    buffer_t* freelist_buffer = NULL;

    for(i=0;i<4;i++)
    {
        temp = (*(p_hashqueue))[i]->buffer_header.next_buffer_on_hashqueue;

        while(temp != (*(p_hashqueue))[i])
        {
            freelist_buffer = search_buffer_in_freelist(p_freelist,temp->buffer_header.block_number);
            if(freelist_buffer != NULL)
            {
                generic_delete_freelist(freelist_buffer);
            }

            next = temp->buffer_header.next_buffer_on_hashqueue;
            temp->buffer_header.p_data = NULL;   

            if(temp->data != NULL)
            {
                free(temp->data);
                temp->data = NULL;
            }

            if(temp != NULL)
            {
                free(temp);
                temp=NULL;
            }

            temp= next;
        }

        free((*(p_hashqueue))[i]);
        (*(p_hashqueue))[i] = NULL;
    }       

    free(p_hashqueue);
    p_hashqueue = NULL;
}
