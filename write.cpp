#include "freelist.h"
#include "hashqueue.h"
#include "iget.h"
#include "getblk.h"
#include "hdd.h"
#include "bwrite.h"
#include "directory_file_rw.h"
#include "iput.h"
#include "write.h"
#include <windows.h>

#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define INVALID_FD -1
#define FILE_DOES_NOT_EXITS -2
#define FILE_PERMISSIONS_NOT_VALID -3
#define INVALID_MODE -4

extern ft_t* ft[MAX_FT_SIZE];
extern ufdt_t* ufdt[MAX_UFDT_SIZE];
extern FILE* fp_log_file;
extern int file_creation_mode;

int check_inode_in_ft_write(int inode_number);

int Write(int fd, char* buffer, int number_of_bytes_to_write)
{
	fprintf(fp_log_file,"------WRITE SYSTEM CALL------\r\n");
	//14 -> 0 17 24 33 32 
	//ceil(log2(n+1)) ceils
	int number_of_blocks_needed;
	status_t s;
	int i;
	int block_number;
	buffer_t* data_buffer = NULL;

	//Check if fd is valid
	if(ufdt[fd] == NULL || ufdt[fd]->ft_pointer == NULL || ufdt[fd]->ft_pointer->iit_pointer == NULL)
	{
		return INVALID_FD;
	} 

	fprintf(fp_log_file,"get the inode %d\r\n",ufdt[fd]->ft_pointer->iit_pointer->inode_number);
	fflush(fp_log_file);
	iit_inode_t* inode = iget(ufdt[fd]->ft_pointer->iit_pointer->inode_number);
	assert(inode != NULL);

	fprintf(fp_log_file,"iget completed\r\n");
	fflush(fp_log_file);
	
	//Check if file exits
	s = directory_file_exits_inode_no(inode->inode_number);
	if(s == 0)
	{
		iput(inode);
		return FILE_DOES_NOT_EXITS;
	}

	//Check if file has write permissions
	if(inode->file_permissions[1] != 1)
	{   
		iput(inode);
		return FILE_PERMISSIONS_NOT_VALID;
	}

	//Check if file is open in WRITE mode
	if(check_inode_in_ft_write(inode->inode_number) == -1)
	{
		iput(inode);
		return INVALID_MODE;
	}

	number_of_blocks_needed = ceil(((float)number_of_bytes_to_write/BLOCK_SIZE));
	fprintf(fp_log_file,"number of blocks needed to be read = %d\r\n", number_of_blocks_needed);
	fflush(fp_log_file);

	if(number_of_bytes_to_write%BLOCK_SIZE == 0)
	{
		for(i=0;i<number_of_blocks_needed;i++)
		{
			block_number = inode->disk_addresses[i];
			//hdd_in_memory_write(block_number,&buffer[i*BLOCK_SIZE],BLOCK_SIZE);
			//printf("\n\nIn Write->\nBlock no = %dData written = %s\n",block_number,buffer);
			fprintf(fp_log_file,"writing to block number = %d\r\n", block_number);
			fflush(fp_log_file);
			data_buffer = getblk(1,block_number);
			assert(data_buffer != NULL);

			data_buffer->buffer_header.device_number = 1; 
			data_buffer->buffer_header.block_number = block_number;
			data_buffer->buffer_header.status = LOCKED ;
			data_buffer->buffer_header.p_data = data_buffer->data; 
			data_buffer->buffer_header.prev_buffer_on_freelist = NULL;
			data_buffer->buffer_header.next_buffer_on_freelist = NULL;

			free(data_buffer->data);
			data_buffer->data = (char*)malloc(17);
			int bakwass = i * BLOCK_SIZE;
			strncpy(data_buffer->data,&buffer[bakwass],BLOCK_SIZE);
			data_buffer->data[BLOCK_SIZE] = '\0';

			bwrite(data_buffer);
		}
	}
	else
	{
		int count=0;
		for(i=0;i<number_of_blocks_needed-1;i++)
		{
			fprintf(fp_log_file,"i = %d\r\n", i);
			fflush(fp_log_file);

			fprintf(fp_log_file,"inode->inode_number = %d inode->disk_addresses[i] = %d\r\n", inode->inode_number, inode->disk_addresses[i]);

			block_number = inode->disk_addresses[i];	
			
			fprintf(fp_log_file,"writing to block number = %d\r\n", block_number);
			fflush(fp_log_file);

			data_buffer = getblk(1,block_number);
			assert(data_buffer != NULL);

			data_buffer->buffer_header.device_number = 1; 
			data_buffer->buffer_header.block_number = block_number;
			data_buffer->buffer_header.status = LOCKED ;
			data_buffer->buffer_header.prev_buffer_on_freelist = NULL;
			data_buffer->buffer_header.next_buffer_on_freelist = NULL;
			
			/*if(data_buffer->data == NULL)
			{
				data_buffer->data = (char*)malloc(16);
				strncpy(data_buffer->data,&buffer[i*BLOCK_SIZE],16);

			}
			else
			{
				data_buffer->data = realloc(data_buffer->data,16);
				strncpy(data_buffer->data,&buffer[i*BLOCK_SIZE],16);	
			}*/

			if(data_buffer->data != NULL)
			{
				free(data_buffer->data);
			}
			
			data_buffer->data = (char*)calloc(17,sizeof(char));
			int bakwass = i * BLOCK_SIZE;
			strncpy(data_buffer->data,&buffer[bakwass],BLOCK_SIZE);
			data_buffer->data[BLOCK_SIZE] = '\0';
			data_buffer->buffer_header.p_data = data_buffer->data; 

			fprintf(fp_log_file,"data buffer->data = %s\n", data_buffer->data);
			fflush(fp_log_file);

			bwrite(data_buffer);

			count++;
		}

		block_number = inode->disk_addresses[count];
		//printf("\n****block_number = %d***",block_number);


		data_buffer = getblk(1,block_number);
		assert(data_buffer != NULL);

		data_buffer->buffer_header.device_number = 1; 
		data_buffer->buffer_header.block_number = block_number;
		data_buffer->buffer_header.status = LOCKED ;
		data_buffer->buffer_header.p_data = data_buffer->data; 
		data_buffer->buffer_header.prev_buffer_on_freelist = NULL;
		data_buffer->buffer_header.next_buffer_on_freelist = NULL;

		/*if(data_buffer->data == NULL)
		{
			data_buffer->data = (char*)malloc(16);
			strncpy(data_buffer->data,&buffer[i*BLOCK_SIZE],number_of_bytes_to_write%BLOCK_SIZE);
		}
		else
		{
			data_buffer->data = realloc(data_buffer->data,16);
			strncpy(data_buffer->data,&buffer[i*BLOCK_SIZE],number_of_bytes_to_write%BLOCK_SIZE);	
		}*/

		if(data_buffer->data != NULL)
		{
			free(data_buffer->data);
		}

		data_buffer->data = (char*)malloc((number_of_bytes_to_write%16)+1);
		//strncpy(data_buffer->data,&buffer[count*BLOCK_SIZE],number_of_bytes_to_write%16);
		fprintf(fp_log_file,"Count = %d",count);
		fflush(fp_log_file);
		int bakwass = count*BLOCK_SIZE;
		strncpy(data_buffer->data,&buffer[bakwass],number_of_bytes_to_write%16);
		
		data_buffer->data[(number_of_bytes_to_write%16)] = '\0';
		fprintf(fp_log_file,"data buffer->data = %s\n", data_buffer->data);
		fflush(fp_log_file);

		bwrite(data_buffer);
		///hdd_in_memory_write(block_number,&buffer[i*BLOCK_SIZE],(strlen(buffer)%16));
	}
	
	fprintf(fp_log_file,"\r\n------WRITE SYSTEM CALL COMPLETED------\r\n");
	fflush(fp_log_file);
	return number_of_bytes_to_write;
}

int check_inode_in_ft_write(int inode_number)
{
	int i;

	fprintf(fp_log_file,"in check_inode\r\n");
	fflush(fp_log_file);
	
    /*for(i=0;i<MAX_FT_SIZE;i++)
    {
		fprintf(fp_log_file,"i<MAX_FT_SIZE\r\n");
		fflush(fp_log_file);
					
        if(ft[i] != NULL && ft[i]->iit_pointer != NULL)
        {
			
			fprintf(fp_log_file,"ft[i] != NULL && ft[i]->iit_pointer \n");
			fflush(fp_log_file);

			if(ft[i]->iit_pointer->inode_number == inode_number)
            {
				fprintf(fp_log_file,"ft[i]->iit_pointer->inode_number \n");
				fflush(fp_log_file);

                if(ft[i]->mode == WRITE)
				{
					fprintf(fp_log_file,"check_inode completed\r\n");
					fflush(fp_log_file);
					return i;
				}
            }
        }
    }*/

	for(i=0;i<MAX_UFDT_SIZE;i++)
    {
		fprintf(fp_log_file,"i<MAX_UFDT_SIZE\r\n");
		fflush(fp_log_file);

		if(ufdt[i] != NULL && ufdt[i]->ft_pointer != NULL && ufdt[i]->ft_pointer->iit_pointer != NULL)
		{
			fprintf(fp_log_file,"ufdt[i] != NULL && ufdt[i]->ft_pointer \n");
			fflush(fp_log_file);

			if(ufdt[i]->ft_pointer->iit_pointer->inode_number ==  inode_number)
			{
				if(ufdt[i]->ft_pointer->mode == WRITE)
				{
					fprintf(fp_log_file,"check_inode completed\r\n");
					fflush(fp_log_file);
					return i;
				}
			}	
		}
					
	}

	fprintf(fp_log_file,"check_inode completed\r\n");
	fflush(fp_log_file);
    return -1;
}