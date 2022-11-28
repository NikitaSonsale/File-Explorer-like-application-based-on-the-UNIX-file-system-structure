#include "dilb.h"
#include <stdio.h>
#include <stdlib.h>

//extern dilb_inode_t* dilb[MAX_DILB_SIZE];

void dilb_initialize()
{
    int i,j;
    for(i=0;i<MAX_DILB_SIZE;i++)
    {
        dilb[i] = (dilb_inode_t*)malloc(sizeof(dilb_inode_t));
        dilb[i]->file_owner = 0;
        dilb[i]->file_size = 0;
        dilb[i]->file_type = 0;
        dilb[i]->inode_number=i;
        
        for(j=0;j<9;j++)
        {
            dilb[i]->file_permissions[j]=0;
        }

        for(j=0;j<10;j++)
        {
            dilb[i]->disk_addresses[j]=0;
        }
    }
}

void dilb_in_memory_write(int inode_number, int file_owner, int file_size, int file_type, int* file_permissions,int* disk_addr)
{
    int j;
;
    for(j=0;j<9;j++)
    {

        dilb[inode_number]->file_permissions[j]=file_permissions[j];
    }

    for(j=0;j<10;j++)
    {
        dilb[inode_number]->disk_addresses[j]=disk_addr[j];
    }

    dilb[inode_number]->file_owner = file_owner;
    dilb[inode_number]->file_size = file_size;
    dilb[inode_number]->file_type = file_type;
    dilb[inode_number]->inode_number = inode_number;
}

dilb_inode_t* dilb_in_memory_read(int inode_number)
{
    return dilb[inode_number];
}

void dilb_disk_write(dilb_inode_t* dilb[MAX_DILB_SIZE])
{
    int i;

    FILE* fd_dilb_file = fopen("DILB.txt","w");
    if(fd_dilb_file == NULL)
    {
        printf("Error:Cannot open data file\n");
        exit(1);
    }    
    
    int j;
    for(i=0;i<MAX_DILB_SIZE;i++)
    {
        fprintf(fd_dilb_file,"%d %d %d %d  ",dilb[i]->inode_number,dilb[i]->file_owner,dilb[i]->file_size,dilb[i]->file_type);

        for(j=0;j<9;j++)
        {
            fprintf(fd_dilb_file,"%d ",dilb[i]->file_permissions[j]);
        }

        fprintf(fd_dilb_file," ");
        
        for(j=0;j<10;j++)
        {
            fprintf(fd_dilb_file,"%d ",dilb[i]->disk_addresses[j]);
        }

        fprintf(fd_dilb_file,"\n");
    }

    fflush(fd_dilb_file);
    fclose(fd_dilb_file);
    fd_dilb_file = NULL;

}

void dilb_disk_read(dilb_inode_t* dilb[MAX_DILB_SIZE])
{
    FILE* fd_dilb_file = fopen("DILB.txt","r");
    if(fd_dilb_file == NULL)
    {
        printf("Error:Cannot open data file\n");
        exit(1);
    }

    int i,j;
    for(i=0;i<MAX_DILB_SIZE;i++)
    {
        fscanf(fd_dilb_file,"%d %d %d %d  ",&dilb[i]->inode_number,&dilb[i]->file_owner,&dilb[i]->file_size,&dilb[i]->file_type);

        for(j=0;j<9;j++)
        {
            fscanf(fd_dilb_file,"%d ",&dilb[i]->file_permissions[j]);
        }
        
        for(j=0;j<10;j++)
        {
            fscanf(fd_dilb_file,"%d ",&dilb[i]->disk_addresses[j]);
        }
    }

    fclose(fd_dilb_file);
    fd_dilb_file = NULL;
}  

void destroy_dilb(dilb_inode_t* dilb[MAX_DILB_SIZE])
{
    int i;
    for(i=0;i<MAX_DILB_SIZE;i++)
    {
        free(dilb[i]);
        dilb[i]=NULL;
    }
}  