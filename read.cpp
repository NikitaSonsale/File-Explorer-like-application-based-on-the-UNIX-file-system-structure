#include "freelist.h"
#include "hashqueue.h"
#include "iget.h"
#include "getblk.h"
#include "hdd.h"
#include "bread.h"
#include "directory_file_rw.h"
#include "iput.h"
#include "read.h"

#include <stdlib.h>
#include <math.h>

#define INVALID_FD -1
#define FILE_DOES_NOT_EXITS -2
#define FILE_PERMISSIONS_NOT_VALID -3
#define INVALID_MODE -4

extern ft_t* ft[MAX_FT_SIZE];
extern ufdt_t* ufdt[MAX_UFDT_SIZE];
extern FILE* fp_log_file;

int check_inode_in_ft_read(int inode_number);

int Read(int fd, char** p_buffer, int number_of_bytes_to_read)
{
	fprintf(fp_log_file,"------READ SYSTEM CALL------\r\n");

	//14 -> 0 17 24 33 32 
	//ceil(log2(n+1)) ceils
	//buffer need not be preallocated
	int number_of_blocks_needed;
	int i;
	int block_number;
	buffer_t* data_buffer = NULL;
	status_t s;

	if(ufdt[fd] == NULL || ufdt[fd]->ft_pointer == NULL || ufdt[fd]->ft_pointer->iit_pointer == NULL)
	{
		return INVALID_FD;
	}

	fprintf(fp_log_file,"get the inode\r\n");
	iit_inode_t* inode = iget(ufdt[fd]->ft_pointer->iit_pointer->inode_number);
	assert(inode != NULL);

	//Check if file exits
	s = directory_file_exits_inode_no(inode->inode_number);
	if(s == 0)
	{
		iput(inode);
		return FILE_DOES_NOT_EXITS;
	}

	//Check if file has write permissions
	if(inode->file_permissions[0] != 1)
	{   
		iput(inode);
		return FILE_PERMISSIONS_NOT_VALID;
	}

	//Check if file is open in WRITE mode
	if(check_inode_in_ft_read(inode->inode_number) == -1)
	{
		iput(inode);
		return INVALID_MODE;
	}

	char** pp_buffer = NULL;
	char* temp_buffer = NULL;

	number_of_blocks_needed = ceil(((float)number_of_bytes_to_read/BLOCK_SIZE));
	fprintf(fp_log_file,"number of blocks needed to be read = %d\r\n", number_of_blocks_needed);

	//printf("number_of_blocks_needed = %d\n",number_of_blocks_needed);
		
	pp_buffer = (char**)malloc(number_of_blocks_needed*sizeof(char*));
	assert(pp_buffer!=NULL);

	if(number_of_bytes_to_read%BLOCK_SIZE == 0)
	{
		for(i=0;i<number_of_blocks_needed;i++)
		{
			block_number = inode->disk_addresses[i];

			fprintf(fp_log_file,"reading block number = %d\r\n", block_number);
			bread(block_number,&data_buffer);

			pp_buffer[i] = (char*)calloc(17,sizeof(char));
			assert(pp_buffer[i] != NULL);

			strncpy(pp_buffer[i],data_buffer->data,16);
			pp_buffer[i][16] = '\0';
		}
	
	}
	else
	{
		int count=0;
		for(i=0;i<number_of_blocks_needed-1;i++)
		{
			block_number = inode->disk_addresses[i];
			bread(block_number,&data_buffer);

			pp_buffer[i] = (char*)calloc(17,1);
			assert(pp_buffer[i] != NULL);

			//fprintf(fp_log_file,"In For: Data buffer->data = %s\n",data_buffer->data);
			//fflush(fp_log_file);

			//strncpy(pp_buffer[i],data_buffer->data,16);
			strncpy(pp_buffer[i],data_buffer->data,16);
			pp_buffer[i][16] = '\0';

			
			fprintf(fp_log_file,"In For: pp_buffer[i] = %s \n",pp_buffer[i]);
			fflush(fp_log_file);

			count++;
		}	

		block_number = inode->disk_addresses[count];

		bread(block_number,&data_buffer);

		pp_buffer[i] = (char*)calloc((number_of_bytes_to_read%16),sizeof(char));
		assert(pp_buffer[i] != NULL);

		fprintf(fp_log_file,"Data buffer->data = %s\n",data_buffer->data);
		fflush(fp_log_file);
		
		strncpy(pp_buffer[i],data_buffer->data,number_of_bytes_to_read%16);
		
		fprintf(fp_log_file,"pp_buffer[i] = %s\n",pp_buffer[i]);
		fflush(fp_log_file);
		
	}
	
	for(i=1;i<number_of_blocks_needed;i++)
	{
		fprintf(fp_log_file,"(strlen(pp_buffer[0]) +  strlen(pp_buffer[i]))= %d\n",(strlen(pp_buffer[0]) +  strlen(pp_buffer[i])));
		fflush(fp_log_file);

		pp_buffer[0] = (char*)realloc(pp_buffer[0],(strlen(pp_buffer[0]) +  strlen(pp_buffer[i])));

		fprintf(fp_log_file,"concatenation  %s %s\n",pp_buffer[0] ,pp_buffer[i]);
		fflush(fp_log_file);

		strcat(pp_buffer[0],pp_buffer[i]);
	}

	fprintf(fp_log_file,"pp_buffer[i] = %s\n",pp_buffer[0]);
	fflush(fp_log_file);


	temp_buffer = (char*)calloc(strlen(pp_buffer[0]) + 1,1);
	assert(temp_buffer!= NULL);

	strncpy(temp_buffer,pp_buffer[0],strlen(pp_buffer[0]));
	//pp_buffer[0] = (char*)realloc(pp_buffer[0],strlen(pp_buffer[0])+1);
	//assert(pp_buffer[0] != NULL);

	fprintf(fp_log_file,"temp_buffer = %s\n",temp_buffer);
	fflush(fp_log_file);

	char *temp = (char*)malloc(1);
	temp[0] = '\0';

	strcat(temp_buffer,temp);

	fprintf(fp_log_file,"temp_buffer = %s\n",temp_buffer);
	fflush(fp_log_file);

	*p_buffer = (char*)calloc(strlen(temp_buffer),strlen(temp_buffer));
	assert(*p_buffer != NULL);

	strcpy(*p_buffer,temp_buffer);

	fprintf(fp_log_file,"*p_buffer = %s\n",*p_buffer);
	fflush(fp_log_file);

	free(pp_buffer);
	pp_buffer = NULL;

	if(temp_buffer!= NULL)
	{
		//free(temp_buffer);
		//temp_buffer = NULL;
	}
	
	fprintf(fp_log_file,"\r\n------READ SYSTEM CALL COMPLETED------\r\n");
	fflush(fp_log_file);
	
	return number_of_bytes_to_read;
}

int check_inode_in_ft_read(int inode_number)
{
	int i;

    /*for(i=0;i<MAX_FT_SIZE;i++)
    {
        if(ft[i] != NULL && ft[i]->iit_pointer != NULL)
        {
            if(ft[i]->iit_pointer->inode_number == inode_number)
            {
                if(ft[i]->mode == READ)
				{
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
				if(ufdt[i]->ft_pointer->mode == READ)
				{
					fprintf(fp_log_file,"check_inode completed\r\n");
					fflush(fp_log_file);
					return i;
				}
			}	
		}
					
	}

    return -1;
}