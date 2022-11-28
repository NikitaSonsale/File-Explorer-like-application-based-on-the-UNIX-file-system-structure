#include "stdio.h"
#include "file_owners.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int check_if_file_owner_exits(char* file_owner)
{
    FILE* fd_file_owners = fopen("file_owners.txt","r");
    assert(fd_file_owners != NULL);

    int no_of_entries;
    fscanf(fd_file_owners,"%d",&no_of_entries);

    char file_o[50];
    int file_owner_number;
    int i = 0;
    while(i<no_of_entries)
    {
        fscanf(fd_file_owners,"%s %d",file_o,&file_owner_number);
        if(strcmp(file_o,file_owner) == 0)
        {
            return TRUE;
        }
        i++;
    }

    return FALSE;
}

int get_file_owner_number(char* file_owner)
{
    FILE* fd_file_owners = fopen("file_owners.txt","r");
    assert(fd_file_owners != NULL);

    int no_of_entries;
    fscanf(fd_file_owners,"%d",&no_of_entries);

    char file_o[50];
    int file_owner_number;
    int i = 0;
    while(i<no_of_entries)
    {
        fscanf(fd_file_owners,"%s %d",file_o,&file_owner_number);
        if(strcmp(file_o,file_owner) == 0)
        {
            return file_owner_number;
        }
        i++;
    }

    return -1;
}

void write_file_owner(char* file_owner,int file_owner_number)
{
    FILE* fd_file_owners = fopen("file_owners.txt","r");

    int no_of_entries;
    fscanf(fd_file_owners,"%d",&no_of_entries);
    int i = 0;

    char** files = (char**)malloc((no_of_entries+1) * sizeof(char*));
    int** number = (int**)malloc((no_of_entries) * sizeof(int*));

    while(i<no_of_entries)
    {
        files[i] = (char*)malloc(50 * sizeof(char));
        number[i] = (int*)malloc(sizeof(int)); 

        fscanf(fd_file_owners,"%s %d",files[i],number[i]);
        i++;
    }
    
    no_of_entries++;
    
    files[i] = file_owner;
    number[i] = (int*)malloc(sizeof(int)); 
    *(number[i]) = file_owner_number;

    fclose(fd_file_owners);
    fd_file_owners = NULL;

    fd_file_owners = fopen("file_owners.txt","w");
    fprintf(fd_file_owners,"%d\n",no_of_entries);

    i = 0;
    while(i<no_of_entries)
    {
        fprintf(fd_file_owners,"%s %d\n",files[i],*(number[i]));
        i++;
    }

    for(i=0;i<no_of_entries;i++)
    {
        free(files[i]);
        free(number[i]);
    }

    fflush(fd_file_owners);
    fclose(fd_file_owners);
    fd_file_owners = NULL;
 
}