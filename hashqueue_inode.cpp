#include "freelist_inode.h"
#include "hashqueue_inode.h"

i_hashqueue_t* i_p_hashqueue = NULL;

i_hashqueue_t* i_create_hashqueue()
{
    i_hashqueue_t* p_hashqueue = (i_hashqueue_t*)malloc(sizeof(i_hashqueue_t));

    int i,j;
    for(i=0;i<4;i++)
    {
        (*(p_hashqueue))[i] = i_get_list_node();   
        (*(p_hashqueue))[i]->device_number = 1;
        (*(p_hashqueue))[i]->file_owner = -1;
        (*(p_hashqueue))[i]->file_size = 0;
        (*(p_hashqueue))[i]->file_type = 0;
        (*(p_hashqueue))[i]->inode_number = INVALID_INODE_NUMBER;
        (*(p_hashqueue))[i]->inode_status = I_UNLOCKED;
        (*(p_hashqueue))[i]->reference_count = 0;
        
        for(j=0;j<9;j++)
        {
           (*(p_hashqueue))[i]->file_permissions[j]=0;
        }

        for(j=0;j<10;j++)
        {
            (*(p_hashqueue))[i]->disk_addresses[j] = 0;
        }

        (*(p_hashqueue))[i]->prev_inode_on_hashqueue = (*(p_hashqueue))[i];
        (*(p_hashqueue))[i]->next_inode_on_hashqueue = (*(p_hashqueue))[i];
        (*(p_hashqueue))[i]->prev_inode_on_freelist = NULL;
        (*(p_hashqueue))[i]->next_inode_on_freelist = NULL;
    }    

    return p_hashqueue;
}

void i_show_hashqueue(i_hashqueue_t* p_hashqueue,const char* msg)
{
    i_node_t *p_run;

    if(msg)
    {
        puts(msg);
    }

    int i;
    i_hashqueueHeaders_t hashqueue ;
    
    for(i=0;i<4;i++)
    {
        hashqueue[i] = (*(p_hashqueue))[i];
        printf("[Beg]<->");
        for(p_run = hashqueue[i]->next_inode_on_hashqueue; p_run != hashqueue[i]; p_run = p_run->next_inode_on_hashqueue)
        {
            printf(" %d <->",p_run->inode_number);
            
        }
        puts("[End]");
    }
}

status_t i_insert_in_hashqueue(i_hashqueue_t* p_hashqueue, iit_inode_t* insert_inode)
{
    i_list_t* p_list ;
    int inode_number = insert_inode->inode_number;

    ///block no -> 10 block_number%4 = 2 (*(p_hashqueue))[2]
    p_list = (*(p_hashqueue))[(inode_number%4)];
    
    //Here new node must not be created. Node from freelist has to be inserted
    i_generic_insert_hashqueue(p_list->prev_inode_on_hashqueue,insert_inode,p_list);

    return SUCCESS;
}

void i_generic_insert_hashqueue(i_node_t *p_beg,i_node_t* p_mid, i_node_t* p_end)
{
    p_mid->next_inode_on_hashqueue = p_end;
    p_mid->prev_inode_on_hashqueue = p_beg;
    p_beg->next_inode_on_hashqueue = p_mid;
    p_end->prev_inode_on_hashqueue = p_mid;
}

iit_inode_t* i_search_in_hashqueue(i_hashqueue_t* p_hashqueue,int inode_number)
{
    i_node_t *p_run;
    i_list_t* p_list ;
    
    p_list = (*(p_hashqueue))[(inode_number%4)];

   // printf("%d %d",p_list->buffer_header.block_number,block_number);
    
    //printf("Returning %s",p_buffer->data);
    for(p_run = p_list->next_inode_on_hashqueue; p_run != p_list; p_run = p_run->next_inode_on_hashqueue)
    {
        if(p_run->inode_number == inode_number)
        {
            return p_run;
        }
    }

    return NULL;
}

void i_generic_delete_hashqueue(iit_inode_t* p_delete_node)
{
    p_delete_node->next_inode_on_hashqueue->prev_inode_on_hashqueue = p_delete_node->prev_inode_on_hashqueue;
    p_delete_node->prev_inode_on_hashqueue->next_inode_on_hashqueue = p_delete_node->next_inode_on_hashqueue;
}

status_t i_delete_from_hashqueue(i_hashqueue_t* p_hashqueue,int inode_number,iit_inode_t** pp_inode)
{
    iit_inode_t* inode_to_delete =  i_search_in_hashqueue(p_hashqueue,inode_number);

    if(inode_to_delete == NULL)
    {
        return INODE_NOT_FOUND;
    }
    else
    {
        *pp_inode = inode_to_delete;
        
        i_generic_delete_hashqueue(inode_to_delete);  
        return SUCCESS; 
    }   
}

status_t i_destroy_hashqueue(i_hashqueue_t** pp_hashqueue)
{
    i_hashqueue_t* p_hashqueue = *pp_hashqueue;

    i_node_t *p_run,*p_run_next;
    i_list_t* p_list ;
    
     
    int i;
    for(i=0;i<4;i++)
    {
        
        p_list = (*(p_hashqueue))[i];
        for(p_run = p_list->next_inode_on_hashqueue; p_run != p_list; p_run = p_run_next)
        {
            p_run_next = p_run->next_inode_on_hashqueue;
           
            if(p_run != NULL)
            {
                free(p_run);
                p_run = NULL;
            }
        }

        free(p_list);
        p_list = NULL;
    }

   
    free(p_hashqueue);
    *pp_hashqueue = NULL;

    return SUCCESS;
}