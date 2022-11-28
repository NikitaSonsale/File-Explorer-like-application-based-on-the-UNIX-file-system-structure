#include "freelist_inode.h"
#include "hashqueue_inode.h"
#include "iget.h"

extern i_hashqueue_t* i_p_hashqueue;
extern i_list_t* i_p_freelist;
extern FILE* fp_log_file;

iit_inode_t* iget(int inode_number)
{
    fprintf(fp_log_file,"------IN IGET------\r\n");
    fflush(fp_log_file);
    
    iit_inode_t* hashqueue_inode = NULL;
    iit_inode_t* freelist_inode = NULL;

    dilb_inode_t* dilb_inode;
    status_t s;
    int i;
   
    while(1)
    {
        fprintf(fp_log_file,"Searching inode %d in hashqueue.\r\n",inode_number);
        fflush(fp_log_file);
        hashqueue_inode = i_search_in_hashqueue(i_p_hashqueue,inode_number);
        
        //Inode is present in hashqueue
        if(hashqueue_inode != NULL)
        {
            fprintf(fp_log_file,"Found inode %d in hashqueue.\r\n",hashqueue_inode->inode_number);
            fflush(fp_log_file);

            freelist_inode = i_search_inode_in_freelist(i_p_freelist,inode_number);

            if(freelist_inode != NULL)
            {
                fprintf(fp_log_file,"Inode %d present on freelist. Deleting inode from freelist.\r\n",hashqueue_inode->inode_number);
                fflush(fp_log_file);

                iit_inode_t* temp_inode = freelist_inode;

                i_generic_delete_freelist(freelist_inode);
                
                if(temp_inode != NULL)
                {
                    fprintf(fp_log_file,"before free %d\r\n",freelist_inode->inode_number);
                    fflush(fp_log_file);

                    //free(temp_inode);

                    fprintf(fp_log_file,"after free");
                    fflush(fp_log_file);

                    //temp_inode = NULL;
                }
            }
           
            /*dilb_inode = dilb_in_memory_read(inode_number);
            hashqueue_inode->device_number = 1;
            hashqueue_inode->file_owner = dilb_inode->file_owner;
            hashqueue_inode->inode_number = inode_number;

            for(i=0;i<9;i++)
            {
                hashqueue_inode->file_permissions[i] = dilb_inode->file_permissions[i];
            }

            for(i=0;i<10;i++)
            {
                hashqueue_inode->disk_addresses[i] = dilb_inode->disk_addresses[i];
            }

            hashqueue_inode->file_size = dilb_inode->file_size;
            hashqueue_inode->file_type = dilb_inode->file_type;
            hashqueue_inode->inode_status = I_LOCKED;
            hashqueue_inode->reference_count = 1;*/
            
/*          freelist_inode->next_inode_on_freelist = NULL;
            freelist_inode->prev_inode_on_freelist = NULL;
            freelist_inode->next_inode_on_hashqueue = NULL;
            freelist_inode->prev_inode_on_hashqueue = NULL;
*/

            //int temp = find_inode_in_iit(hashqueue_inode->inode_number);

            //if(temp == -1)
            
            fprintf(fp_log_file,"------IGET COMPLETED------\r\n");
            fflush(fp_log_file);
            
            return hashqueue_inode;
        }
        else
        {
            fprintf(fp_log_file,"Inode %d is not hashqueue.\r\n",inode_number);
            fflush(fp_log_file);

            freelist_inode = i_search_inode_in_freelist(i_p_freelist,inode_number);
            if(freelist_inode != NULL)
            {
                i_generic_delete_freelist(freelist_inode);
                hashqueue_inode = freelist_inode;
            }
            else
            {
                hashqueue_inode = i_get_list_node();    
            }

            fprintf(fp_log_file,"Remove old inode %d from hashqueue.\r\n",hashqueue_inode->inode_number);
            fflush(fp_log_file);

            i_delete_from_hashqueue(i_p_hashqueue,inode_number,&hashqueue_inode);

            fprintf(fp_log_file,"Read inode from disk.\r\n");
            fflush(fp_log_file);

            dilb_inode = dilb_in_memory_read(inode_number);
            assert(dilb_inode!=NULL);

            hashqueue_inode->device_number = 1;
            hashqueue_inode->file_owner = dilb_inode->file_owner;
            hashqueue_inode->inode_number = dilb_inode->inode_number;

            for(i=0;i<9;i++)
            {
                hashqueue_inode->file_permissions[i] = dilb_inode->file_permissions[i];
            }

            for(i=0;i<10;i++)
            {
                hashqueue_inode->disk_addresses[i] = dilb_inode->disk_addresses[i];
            }

            hashqueue_inode->file_size = dilb_inode->file_size;
            hashqueue_inode->file_type = dilb_inode->file_type;
            hashqueue_inode->inode_status = I_LOCKED;
            hashqueue_inode->reference_count = 1;
            //hashqueue_inode->link_count = 1;
            
            hashqueue_inode->next_inode_on_freelist = NULL;
            hashqueue_inode->prev_inode_on_freelist = NULL;
            hashqueue_inode->next_inode_on_hashqueue = NULL;
            hashqueue_inode->prev_inode_on_hashqueue = NULL;


            //int temp = find_inode_in_iit(hashqueue_inode->inode_number);

            //if(temp == -1)
            
           // i_insert_inode_in_iit(hashqueue_inode);

            fprintf(fp_log_file,"Insert new inode %d in hashqueue.\r\n",hashqueue_inode->inode_number);
            fflush(fp_log_file);
            
            i_insert_in_hashqueue(i_p_hashqueue,hashqueue_inode);

            fprintf(fp_log_file,"------IGET COMPLETED------\r\n");
            fflush(fp_log_file);
            
            return hashqueue_inode;
        }
    }

    return NULL;
}