#include "hdd.h"

void hdd_initialize()
{
    int i;
    //printf("block size = %d\n",BLOCK_SIZE);
    for(i=0;i<HDD_SIZE;i++)
    {
        hdd[i] = (char*)calloc((BLOCK_SIZE),sizeof(char));
        //hdd[i] = (char*)malloc((BLOCK_SIZE));
        //hdd[i][BLOCK_SIZE] = '\n';
        //hdd[i][BLOCK_SIZE+1] = '\0';
    }
    //hdd[0] = (char*)calloc((BLOCK_SIZE),sizeof(char));
    
}

void hdd_in_memory_write(int block_number, char* data,int no_of_bytes)
{
    //Precondition: strlen(data) <= 16
    
    int i;
    //hdd[block_number] = (char*)malloc(no_of_bytes);
    //assert(hdd[block_number] != NULL);

    /*for(i=0;i<no_of_bytes;i++)
    {
        hdd[block_number][i] = data[i];    
    }*/
    
    for(i=0;i<BLOCK_SIZE;i++)
    {
        hdd[block_number][i] = '\0';    
    }
    
    strncpy(hdd[block_number],data,no_of_bytes);
}

char* hdd_in_memory_read(int block_number)
{
    return hdd[block_number];
}

void hdd_disk_write(char* hdd[HDD_SIZE])
{
    int i;

    FILE* fd_data_file = fopen("data_file.txt","w");
    if(fd_data_file == NULL)
    {
        printf("Error:Cannot open data file\n");
        exit(1);
    }    
    
    for(i=0;i<HDD_SIZE;i++)
    {
        fwrite(hdd[i],BLOCK_SIZE,1,fd_data_file);
        //fprintf(fd_data_file,"%s",hdd[i]);
    }
    
    //fwrite(hdd[0],BLOCK_SIZE,1,fd_data_file);
    //fprintf(fd_data_file,"%s",hdd[0]);

    fflush(fd_data_file);
    fclose(fd_data_file);
    fd_data_file = NULL;

}

void hdd_disk_read(char* hdd[HDD_SIZE])
{
    FILE* fd_data_file = fopen("data_file.txt","r");
    if(fd_data_file == NULL)
    {
        printf("Error:Cannot open data file\n");
        exit(1);
    }

    int i;

    for(i=0;i<HDD_SIZE;i++)
    {
        fread(hdd[i],BLOCK_SIZE,1,fd_data_file);
    }

    fclose(fd_data_file);
    fd_data_file = NULL;
}

void destroy_in_memory_hdd()
{
    int i;

    for(i=0;i<HDD_SIZE;i++)
    {
        free(hdd[i]);
        hdd[i]=NULL;
    }
}
