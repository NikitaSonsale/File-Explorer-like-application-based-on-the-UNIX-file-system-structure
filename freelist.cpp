#include "freelist.h"

list_t* p_freelist=NULL;

list_t *create_list()
{
    //node_t = list_t = buffer_t = struct Buffer {buffer_header , data}
    node_t* p_new_node = NULL;
    
    p_new_node = get_list_node(NULL);
    p_new_node->buffer_header.device_number = 1;
    p_new_node->buffer_header.block_number = INVALID_BLOCK_NUMBER;
    p_new_node->buffer_header.status = UNLOCKED;
    p_new_node->buffer_header.p_data = p_new_node->data;
    p_new_node->buffer_header.prev_buffer_on_hashqueue = NULL;
    p_new_node->buffer_header.next_buffer_on_hashqueue = NULL;
    p_new_node->buffer_header.prev_buffer_on_freelist = p_new_node;
    p_new_node->buffer_header.next_buffer_on_freelist = p_new_node;

    p_new_node->data = (char*)malloc(2);
    //itoa(0,p_new_node->data,10);
    sprintf(p_new_node->data,"%d",0);

    return p_new_node;
}

node_t* get_list_node(data_t data)
{
    node_t* temp_node = NULL;
    temp_node = (node_t*)xmalloc(sizeof(node_t));
    assert(temp_node != NULL);
   
    if(data == NULL)
    {
        temp_node->data = data;
    }
    else
    {
        temp_node->data = (char*)malloc(BUFFERDATASIZE*sizeof(char));
        //temp_node->data = memcpy(temp_node->data,data,BUFFERDATASIZE);
        strcpy(temp_node->data,data);
    }

    //printf("get_list_node = %s\n",temp_node->data);
    return temp_node;
}

void* xmalloc(int size)
{
    void* p = NULL;
    p = malloc(size);
    if(p == NULL)
    {
        printf("Malloc:Out Of Memory!");
        exit(-1);
    }
    
    return p;
}

status_t insert_end(list_t* p_list,buffer_t* insert_buffer)
{
    generic_insert(p_list->buffer_header.prev_buffer_on_freelist  ,insert_buffer ,p_list);
    int number_of_nodes = atoi(p_list->data);
    number_of_nodes++;
    //itoa(number_of_nodes,p_list->data,10);
    sprintf(p_list->data,"%d",number_of_nodes);
    
    return SUCCESS;
}

status_t insert_beg(list_t* p_list,buffer_t* insert_buffer)
{
    generic_insert(p_list,insert_buffer,p_list->buffer_header.next_buffer_on_freelist);
    int number_of_nodes = atoi(p_list->data);
    number_of_nodes++;
    //itoa(number_of_nodes,p_list->data,10);
    sprintf(p_list->data,"%d",number_of_nodes);

    return SUCCESS;    
}

void generic_insert(node_t *p_beg,node_t* p_mid, node_t* p_end)
{
    p_mid->buffer_header.next_buffer_on_freelist = p_end;
    p_mid->buffer_header.prev_buffer_on_freelist = p_beg;
    p_beg->buffer_header.next_buffer_on_freelist = p_mid;
    p_end->buffer_header.prev_buffer_on_freelist = p_mid;
}


status_t show_freelist(list_t* p_list,const char* msg)
{
    node_t *p_run;
    if(msg)
    {
        puts(msg);
    }
    printf("[Beg]<->");
    
    
    for(p_run = p_list->buffer_header.next_buffer_on_freelist; p_run != p_list; p_run = p_run->buffer_header.next_buffer_on_freelist)
    {
        printf(" %d <->",p_run->buffer_header.block_number);
    }
    puts("[End]");

    return SUCCESS;
}

bool is_freelist_empty(list_t* p_list)
{
    int number_of_nodes = atoi(p_list->data);
    
    if(number_of_nodes == FREELISTSIZE)
        return TRUE;
    else
        return FALSE;
}

len_t get_freelist_length(list_t* p_list)
{
    node_t* p_run;
    len_t len=0;


    for(p_run = p_list->buffer_header.next_buffer_on_freelist;p_run != p_list;p_run = p_run->buffer_header.next_buffer_on_freelist)
    {
        if(p_run->data != NULL)
        {
            len = len+1;
        }
    }

    return len;
}

status_t delete_from_freelist_beg(list_t* p_list,buffer_t** pp_buffer)
{
    /*if(is_freelist_empty(p_list) == TRUE)
    {
        return LIST_EMPTY;
    }*/

    *pp_buffer = p_list->buffer_header.next_buffer_on_freelist;

    // buffer_t* buffer_to_delete = p_list->buffer_header.next_buffer_on_freelist;

    // buffer_t* buffer = get_list_node(buffer_to_delete->data);
    // buffer->buffer_header.block_number = buffer_to_delete->buffer_header.block_number;
    // buffer->buffer_header.device_number = buffer_to_delete->buffer_header.device_number;
    // buffer->buffer_header.status = buffer_to_delete->buffer_header.status;
    // buffer->buffer_header.next_buffer_on_freelist = NULL;
    // buffer->buffer_header.prev_buffer_on_freelist = NULL;
    // buffer->buffer_header.next_buffer_on_hashqueue = buffer_to_delete->buffer_header.next_buffer_on_hashqueue;
    // buffer->buffer_header.prev_buffer_on_hashqueue = buffer_to_delete->buffer_header.prev_buffer_on_hashqueue;

    // memcpy(buffer->data,buffer_to_delete->data,strlen(buffer_to_delete->data));
    // //printf("bd = %s",buffer->data);
    
    generic_delete_freelist(p_list->buffer_header.next_buffer_on_freelist);

    return SUCCESS;
}

void generic_delete_freelist(buffer_t* p_delete_node)
{   
    p_delete_node->buffer_header.next_buffer_on_freelist->buffer_header.prev_buffer_on_freelist =  p_delete_node->buffer_header.prev_buffer_on_freelist;
    p_delete_node->buffer_header.prev_buffer_on_freelist->buffer_header.next_buffer_on_freelist =  p_delete_node->buffer_header.next_buffer_on_freelist;
}

buffer_t* search_buffer_in_freelist(list_t* p_freelist,int block_number)
{
    buffer_t* p_run = NULL;
    for(p_run = p_freelist->buffer_header.next_buffer_on_freelist; p_run != p_freelist; p_run = p_run->buffer_header.next_buffer_on_freelist)
    {
        if(p_run->buffer_header.block_number == block_number)
        {
            return p_run;
        }
    }

    return NULL;
}

status_t initialize_freelist(list_t* p_freelist)
{
    int i;
    status_t s;
    node_t* new_buffer;
    for(i = 0;i < FREELISTSIZE;i++)
    {   
        new_buffer = get_list_node(NULL);
        assert(new_buffer != NULL);

        new_buffer->buffer_header.device_number = 1;
        new_buffer->buffer_header.block_number = -1;
        new_buffer->buffer_header.status = 0;
        new_buffer->buffer_header.p_data = new_buffer->data;
        new_buffer->buffer_header.prev_buffer_on_hashqueue = NULL;
        new_buffer->buffer_header.next_buffer_on_hashqueue = NULL;

        generic_insert(p_freelist->buffer_header.prev_buffer_on_freelist,new_buffer,p_freelist);
    }

    return SUCCESS;
}

void destroy_freelist(list_t* p_list)
{
    //printf("In destroy %d\n",p_list->buffer_header.next_buffer_on_freelist->buffer_header.block_number);
    node_t* temp = p_list->buffer_header.next_buffer_on_freelist;
    node_t* next_node = NULL;

    //printf("next node = %d\n",next_node->buffer_header.block_number);
    while(temp != p_list)
    {
        //printf("freeing %d\n",temp->buffer_header.block_number);

        next_node = temp->buffer_header.next_buffer_on_freelist;

        //printf("next node = %d\n",next_node->buffer_header.block_number);

        if(temp->buffer_header.status == DELAYED_WRITE)
        {
             printf("Delayed write buffer %d written to disk = %s\n",temp->buffer_header.block_number,temp->data);
             hdd_in_memory_write(temp->buffer_header.block_number,temp->data,strlen(temp->data));
        }

        //Safe release
        if(temp->data != NULL)
        {
            free(temp->data);
            temp->data = NULL;
        }

        if(temp != NULL)
        {
            free(temp);
            temp = NULL;
        }
        
        temp = next_node;
    }

    if(p_list != NULL)
    {
        free(p_list->data);
        p_list->data = NULL;
        
        free(p_list);
        p_list=NULL;    
    }
}
