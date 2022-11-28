#include "inodecache.h"
#include "freelist_inode.h"
#include "hashqueue_inode.h"

//extern int index_in_iit;
extern iit_inode_t* incore_inode_table[MAX_IIT_SIZE];
extern ufdt_t* ufdt[MAX_UFDT_SIZE];
extern ft_t* ft[MAX_FT_SIZE];
extern i_list_t* i_p_freelist;
extern FILE* fp_log_file;

/*void i_insert_inode_in_iit(iit_inode_t* inode)
{
    int index;
    if(check_inode_in_ufdt(inode,&index) == TRUE)
    {    
        //printf("here\n");
        index = find_inode_in_iit(inode->inode_number);
         
        incore_inode_table[index]->reference_count++;
        //inode->reference_count++;
    }
    else
    {
        //printf("there\n");
        index = find_free_slot_in_IIT();
        incore_inode_table[index] = inode; 
    }
}*/

void i_insert_inode_in_iit(iit_inode_t* inode)
{
    int index;
    index = find_inode_in_iit(inode->inode_number);
    
    if(index != -1)
    {
        //fprintf(fp_log_file,"Inode %d already present on IIT. Incremention reference count\r\n",inode->inode_number);
        //fflush(fp_log_file);
        incore_inode_table[index]->reference_count++; 

        fprintf(fp_log_file,"Inode %d already present on IIT. Incremention reference count.\r\n Ref Count = %d",inode->inode_number,incore_inode_table[index]->reference_count);
        fflush(fp_log_file);
       
    }     
    else
    {
        fprintf(fp_log_file,"Inserting inode %d in IIT\r\n",inode->inode_number);
        fflush(fp_log_file);

        index = find_free_slot_in_IIT();
        incore_inode_table[index] = inode; 
    }
}

int find_free_slot_in_IIT()
{
    int i;
    for(i=0;i<MAX_IIT_SIZE;i++)
    {
        if(incore_inode_table[i] == NULL)
        {
            return i;
        }
    }

    return -1;
}

int find_free_slot_in_FT()
{
    int i;
    for(i=0;i<MAX_FT_SIZE;i++)
    {
        if(ft[i] == NULL)
        {
            return i;
        }
    }

    return -1;
}

int find_free_slot_in_UFDT()
{
    int i;
    for(i=0;i<MAX_IIT_SIZE;i++)
    {
        if(ufdt[i] == NULL)
        {
            return i;
        }
    }

    return -1;
}

status_t check_inode_in_ufdt(iit_inode_t* inode,int* index)
{
    int i;
    for(i=0;i<MAX_UFDT_SIZE;i++)
    {
        if(ufdt[i] != NULL)
        {
            if(ufdt[i]->ft_pointer->iit_pointer->inode_number == inode->inode_number)
            {
                *index = i;
                return TRUE;
            }
        }
    }

    return FALSE;
}

int find_inode_in_iit(int inode_number)
{
    int i;
     //printf("there");
    for(i=0;i<MAX_IIT_SIZE;i++)
    {
        if(incore_inode_table[i] != NULL)
        {
             //printf("there");
            if(incore_inode_table[i]->inode_number == inode_number)
            {
                 //printf("there");
                return i;
            }
        }
    }

    return -1;
}

int find_inode_in_ft(int inode_number)
{
    int i;

    for(i=0;i<MAX_IIT_SIZE;i++)
    {
        if(ft[i] != NULL && ft[i]->iit_pointer != NULL)
        {
            if(ft[i]->iit_pointer->inode_number == inode_number)
            {
                return i;
            }
        }
    }

    return -1;
}

int find_inode_in_ufdt(int inode_number)
{
    int i;

    for(i=0;i<MAX_UFDT_SIZE;i++)
    {
        if(ufdt[i] != NULL && ufdt[i]->ft_pointer != NULL && ufdt[i]->ft_pointer->iit_pointer != NULL)
        {
            if(ufdt[i]->ft_pointer->iit_pointer->inode_number == inode_number)
            {
                return i;
            }
        }
    }

    return -1;
}

void i_show_iit(iit_inode_t* incore_inode_table[MAX_IIT_SIZE],const char* msg)
{
    printf("\n%s\n",msg);
    iit_inode_t* inode = NULL;

    int i;
    // for(i=0; i < MAX_FT_SIZE;i++)
    // {
    //     if(ft[i] != NULL && ufdt[i] != NULL )
    //     {
    //         printf("%d   ",ft[i]->iit_pointer->inode_number);
    //     }
    // }

    for(i=0; i < MAX_IIT_SIZE;i++)
    {
        //printf("show_iit");
        if(incore_inode_table[i] != NULL  )
        {
            //printf("show_iit up= %d   ",incore_inode_table[i]->inode_number);
            if(incore_inode_table[i]->file_type != 0)
            {
                //printf("show_iit down= %d   ",incore_inode_table[i]->inode_number);
                inode = i_search_inode_in_freelist(i_p_freelist,incore_inode_table[i]->inode_number);
                if(inode != NULL)
                    continue;

                printf("%d   ",incore_inode_table[i]->inode_number);
            }
        }
    }
}

void i_show_ft(ft_t* ft[MAX_FT_SIZE],const char* msg)
{
    printf("\n%s\n",msg);

    int i;
    for(i=0; i < MAX_FT_SIZE;i++)
    {
        if(ft[i] != NULL && ufdt[i] != NULL )
        {
            printf("%d      ",ft[i]->iit_pointer->inode_number);
        }
    }

    printf("\n");
}

void i_show_ufdt(ufdt_t* ufdt[MAX_UFDT_SIZE],const char* msg)
{
    printf("\n%s\n",msg);

    int i;
    for(i=0;i < MAX_UFDT_SIZE;i++)
    {
        if(ufdt[i] != NULL && ufdt[i]->ft_pointer!= NULL && ufdt[i]->ft_pointer->iit_pointer!=NULL) //&& ufdt[i]->ft_pointer!= NULL && ufdt[i]->ft_pointer->iit_pointer!=NULL
        {
            printf("%d      ",ufdt[i]->ft_pointer->iit_pointer->inode_number);
        }
    }

    printf("\n");
}