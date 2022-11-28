#include "freelist_inode.h"
#include "hashqueue_inode.h"
#include "free.h"
#include "ifree.h"
#include "freelist.h"
#include "hashqueue.h"

extern i_hashqueue_t* i_p_hashqueue;
extern i_list_t* i_p_freelist;
extern hashqueue_t* p_hashqueue;
extern int index_in_iit;
extern FILE* fp_log_file;

void iput(iit_inode_t* in_core_inode)
{
    fprintf(fp_log_file,"------IN IPUT------\r\n");

    fprintf(fp_log_file,"Locked Inode. Decrementing reference count.\r\n");
    fflush(fp_log_file);

    in_core_inode->inode_status = I_LOCKED;
    in_core_inode->reference_count = in_core_inode->reference_count-1;

    fprintf(fp_log_file,"Locked Inode. Decrementing reference count. Ref count = %d\r\n",in_core_inode->reference_count);
    fflush(fp_log_file);

    //in_core_inode->link_count = in_core_inode->link_count-1;
 
    int i;
    if(in_core_inode->reference_count == 0)
    {
        //if(in_core_inode->link_count == 0)
        //{
            /*for(i=0;i<10;i++)
            {
                if(in_core_inode->disk_addresses[i] != INVALID_DISK_ADDRESS)
                {
                    fprintf(fp_log_file,"Reference count and link count = 0.\r\nFreeing disk blocks\r\n");
                    fflush(fp_log_file);

                    block_free(in_core_inode->disk_addresses[i]);
                    in_core_inode->disk_addresses[i] = INVALID_DISK_ADDRESS;
                }
            }  

            fprintf(fp_log_file,"Set File type = 0\r\n");
            fflush(fp_log_file);
            in_core_inode->file_type = 0;

            fprintf(fp_log_file,"Free Inode\r\n");
            fflush(fp_log_file);
            ifree(in_core_inode->inode_number);
        //}*/
        
        //dilb_in_memory_write(in_core_inode->inode_number,in_core_inode->file_owner,in_core_inode->file_size,
                            //in_core_inode->file_type,in_core_inode->file_permissions,in_core_inode->disk_addresses);

        /*for(i=0;i<9;i++)
        {
            in_core_inode->file_permissions[i] = 0;
        }

        in_core_inode->file_type = 0;
        in_core_inode->file_owner = 0;
        in_core_inode->file_size = 0;*/

        
        //in_core_inode->inode_status = I_UNLOCKED;
        
        /*iit_inode_t* inode = i_get_list_node();
        iit_inode_t* inode2 = i_get_list_node();

        inode->inode_status = I_UNLOCKED;
        inode->reference_count = 0;
        inode->file_type = 0;
        inode->file_size = 0;

        for(i=0;i<10;i++)
        {
            inode->disk_addresses[i] = 0;
        }
        inode->inode_number = in_core_inode->inode_number;
        inode->prev_inode_on_freelist = NULL;
        inode->next_inode_on_freelist = NULL;
        inode->prev_inode_on_hashqueue = NULL;
        inode->next_inode_on_freelist = NULL;*/

        fprintf(fp_log_file,"Inserting %d at end of inode freelist\r\n",in_core_inode->inode_number);
        i_insert_end(i_p_freelist,in_core_inode);

        //int index = find_inode_in_iit(in_core_inode->inode_number);
        
        //i_delete_from_hashqueue(i_p_hashqueue,in_core_inode->inode_number,&inode2);

        //free(incore_inode_table[index]);
        //incore_inode_table[index] = NULL;

        //i_insert_in_hashqueue(i_p_hashqueue,inode); 
         
    }
    //else
    //{
        //dilb_in_memory_write(in_core_inode->inode_number,in_core_inode->file_owner,in_core_inode->file_size,in_core_inode->file_type,in_core_inode->file_permissions,in_core_inode->disk_addresses);
        
    //}
 
    in_core_inode->inode_status = I_UNLOCKED;

    fprintf(fp_log_file,"Inode status = UNLOCKED\r\n");
    fprintf(fp_log_file,"------IPUT COMPLETED------\r\n");
    
    fflush(fp_log_file);
}