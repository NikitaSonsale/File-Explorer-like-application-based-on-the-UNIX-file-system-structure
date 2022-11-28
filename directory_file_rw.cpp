#include "directory_file_rw.h"
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_NAME_LEN 50


status_t directory_file_read(char* file_name,int* inode_number)
{
    FILE* fd_directory_file = fopen("directory_file.txt","r");

    char file[MAX_FILE_NAME_LEN];
    int inode;
    int no_of_entries;
    fscanf(fd_directory_file,"%d",&no_of_entries);

    int i = 0;
    while(i<no_of_entries)
    {
        fscanf(fd_directory_file,"%s %d",file,&inode);
        
        if(strcmp(file,file_name) == 0)
        {   
            *inode_number = inode;
            
            fclose(fd_directory_file);
            fd_directory_file = NULL;
            
            return 0;
        }
        i++;
    }

    fclose(fd_directory_file);
    fd_directory_file = NULL;

    return -1;
}

status_t directory_file_write(char* file_name,int inode_number)
{
    FILE* fd_directory_file = fopen("directory_file.txt","r");

    int no_of_entries;
    fscanf(fd_directory_file,"%d",&no_of_entries);
    
    int i = 0;

    //char** files = (char**)malloc((no_of_entries+1) * sizeof(char*));
    char** files = (char**)malloc((no_of_entries) * sizeof(char*));
    int** inodes = (int**)malloc((no_of_entries) * sizeof(int*));

    while(i<no_of_entries)
    {
        files[i] = (char*)malloc(MAX_FILE_NAME_LEN * sizeof(char));
        inodes[i] = (int*)malloc(sizeof(int)); 

        fscanf(fd_directory_file,"%s %d",files[i],inodes[i]);
        i++;
    }
    
    no_of_entries++;
    
    //files[i] = file_name;
    files = (char**)realloc(files,(no_of_entries)*sizeof(char*));
    assert(files != NULL);

    //inodes[i] = (int*)malloc(sizeof(int)); 
    inodes = (int**)realloc(inodes,(no_of_entries)*sizeof(int*));
    assert(inodes != NULL);

    //printf("\nI am here\n");
    files[i] = (char*)malloc(MAX_FILE_NAME_LEN*sizeof(char));
    strcpy(files[i],file_name);

    //files[i] = file_name;
    //printf("\nI am reaching here\n");
    inodes[i] = (int*)malloc(sizeof(int)); 
    *(inodes[i]) = inode_number;
    //printf("\nI am not reaching here\n");

    fclose(fd_directory_file);
    fd_directory_file = NULL;

    fd_directory_file = fopen("directory_file.txt","w");
    fprintf(fd_directory_file,"%d\n",no_of_entries);

    i = 0;
    while(i<no_of_entries-1)
    {
        fprintf(fd_directory_file,"%s %d\n",files[i],*(inodes[i]));
        i++;
    }  

    fprintf(fd_directory_file,"%s %d",files[no_of_entries-1],*(inodes[no_of_entries-1]));                                                                     

    for(i=0;i<no_of_entries;i++)
    {
        free(files[i]);
        files[i]= NULL;

        free(inodes[i]);
        inodes[i] = NULL;
    }

    free(files);
    files = NULL;

    free(inodes);
    inodes = NULL;

    fflush(fd_directory_file);
    fclose(fd_directory_file);
    fd_directory_file = NULL;

    return 0;
    //fprintf(fd_directory_file,"%s ")
}

int directory_file_exits(char* filename)
{
    FILE* fd_directory_file = fopen("directory_file.txt","r");

    char file[MAX_FILE_NAME_LEN];
    int inode;
    int no_of_entries;
    fscanf(fd_directory_file,"%d",&no_of_entries);

    int i = 0;
    while(i<no_of_entries)
    {
        fscanf(fd_directory_file,"%s %d",file,&inode);
        if(strcmp(file,filename) == 0)
        {
            fclose(fd_directory_file);
            fd_directory_file = NULL;
            return 1;
        }
        i++;
    }
    
    fclose(fd_directory_file);
    fd_directory_file = NULL;

    return 0;
}

int directory_file_exits_inode_no(int inode_number)
{
    FILE* fd_directory_file = fopen("directory_file.txt","r");

    char file[MAX_FILE_NAME_LEN];
    int inode;
    int no_of_entries;
    fscanf(fd_directory_file,"%d",&no_of_entries);

    int i = 0;
    while(i<no_of_entries)
    {
        fscanf(fd_directory_file,"%s %d",file,&inode);
        if(inode == inode_number)
        {
            fclose(fd_directory_file);
            fd_directory_file = NULL;
            return 1;
        }
        i++;
    }
    
    fclose(fd_directory_file);
    fd_directory_file = NULL;

    return 0;
}