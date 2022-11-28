#include "freelist_inode.h"

i_list_t* i_p_freelist=NULL;

i_list_t *i_create_list()
{
    //node_t = list_t = buffer_t = struct Buffer {buffer_header , data}
    i_node_t* p_new_node = NULL;
    
    p_new_node = i_get_list_node();
    p_new_node->device_number = 1;
    p_new_node->file_owner = -1;
    p_new_node->file_size = 0;
    p_new_node->file_type = 0;
    p_new_node->reference_count = 0;
    
    int i;
    for(i=0;i<9;i++)
    {
        p_new_node->file_permissions[i] = 0;
    }

    for(i=0;i<10;i++)
    {
        p_new_node->disk_addresses[i] = -0;
    }
    p_new_node->inode_status = I_UNLOCKED;
    p_new_node->inode_number = INVALID_INODE_NUMBER;
    p_new_node->prev_inode_on_hashqueue = NULL;
    p_new_node->next_inode_on_hashqueue = NULL;
    p_new_node->prev_inode_on_freelist = p_new_node;
    p_new_node->next_inode_on_freelist = p_new_node;

    return p_new_node;
}

i_node_t* i_get_list_node()
{
    i_node_t* temp_node = NULL;
    temp_node = (i_node_t*)malloc(sizeof(i_node_t));
    assert(temp_node != NULL);
   
    return temp_node;
}

status_t i_insert_end(i_list_t* p_list,iit_inode_t* insert_buffer)
{
    i_generic_insert(p_list->prev_inode_on_freelist  ,insert_buffer ,p_list);
    
    return SUCCESS;
}

status_t i_insert_beg(i_list_t* p_list,iit_inode_t* insert_buffer)
{
    i_generic_insert(p_list,insert_buffer,p_list->next_inode_on_freelist);

    return SUCCESS;    
}


void i_generic_insert(i_node_t *p_beg,i_node_t* p_mid, i_node_t* p_end)
{
    p_mid->next_inode_on_freelist = p_end;
    p_mid->prev_inode_on_freelist = p_beg;
    p_beg->next_inode_on_freelist = p_mid;
    p_end->prev_inode_on_freelist = p_mid;
}

status_t i_show_freelist(i_list_t* p_list,const char* msg)
{
    i_node_t *p_run;
    if(msg)
    {
        puts(msg);
    }
    printf("[Beg]<->");
    for(p_run = p_list->next_inode_on_freelist; p_run != p_list; p_run = p_run->next_inode_on_freelist)
    {
        printf(" %d <->",p_run->inode_number);
    }
    puts("[End]");

    return SUCCESS;
}

int i_is_freelist_empty(i_list_t* p_list)
{
    if(p_list->next_inode_on_freelist == p_list && p_list->prev_inode_on_freelist == p_list)
        return TRUE;
    else
        return FALSE;    
}

status_t i_delete_from_freelist_beg(i_list_t* p_list,iit_inode_t** pp_inode)
{
    if(i_is_freelist_empty(p_list) == TRUE)
    {
        return LIST_EMPTY;
    }

    *pp_inode = p_list->next_inode_on_freelist;
    
    i_generic_delete_freelist(p_list->next_inode_on_freelist);

    return SUCCESS;
}

void i_generic_delete_freelist(iit_inode_t* p_delete_node)
{   
    p_delete_node->next_inode_on_freelist->prev_inode_on_freelist =  p_delete_node->prev_inode_on_freelist;
    p_delete_node->prev_inode_on_freelist->next_inode_on_freelist =  p_delete_node->next_inode_on_freelist;
}

status_t i_initialize_freelist(i_list_t* p_freelist)
{
    int i;
    status_t s;
    i_node_t* p_new_node;
    for(i = 0;i < FREELISTSIZE;i++)
    {   
        p_new_node = i_get_list_node();
        assert(p_new_node != NULL);

        p_new_node->device_number = 1;
        p_new_node->file_owner = -1;
        p_new_node->file_size = 0;
        p_new_node->file_type = 0;
        p_new_node->inode_status = UNLOCKED;
        p_new_node->reference_count = 0;
        
        int i;
        for(i=0;i<9;i++)
        {
            p_new_node->file_permissions[i] = 0;
        }

        for(i=0;i<10;i++)
        {
            p_new_node->disk_addresses[i] = 0;
        }
        p_new_node->inode_status = UNLOCKED;
        p_new_node->inode_number = INVALID_INODE_NUMBER;
        p_new_node->prev_inode_on_hashqueue = NULL;
        p_new_node->next_inode_on_hashqueue = NULL;
        p_new_node->prev_inode_on_freelist = p_new_node;
        p_new_node->next_inode_on_freelist = p_new_node;

        i_generic_insert(p_freelist->prev_inode_on_freelist,p_new_node,p_freelist);
    }

    return SUCCESS;
}

iit_inode_t* i_search_inode_in_freelist(i_list_t* p_freelist,int inode_number)
{
    iit_inode_t* p_run = NULL;
    for(p_run = p_freelist->next_inode_on_freelist; p_run != p_freelist; p_run = p_run->next_inode_on_freelist)
    {
        if(p_run->inode_number == inode_number)
        {
            return p_run;
        }
    }

    return NULL;
}

status_t i_destroy_freelist(i_list_t** pp_freelist)
{
    i_list_t* p_freelist = *pp_freelist;

    iit_inode_t* p_run = NULL;
    iit_inode_t* p_run_next = NULL;

    for(p_run = p_freelist->next_inode_on_freelist; p_run != p_freelist; p_run = p_run_next)
    {
       p_run_next = p_run->next_inode_on_freelist;

       if(p_run != NULL)
       {
            free(p_run);
            p_run = NULL;
       }
       
    }

    free(p_freelist);
    *pp_freelist = NULL;

    return SUCCESS;

}