#include "client.h"

extern hashqueue_t* p_hashqueue;
extern list_t* p_freelist;
extern i_list_t* i_p_freelist;
extern i_hashqueue_t* i_p_hashqueue;
extern dilb_inode_t* dilb[MAX_DILB_SIZE];
extern iit_inode_t* incore_inode_table[MAX_IIT_SIZE];
extern ufdt_t* ufdt[MAX_UFDT_SIZE];
extern ft_t* ft[MAX_FT_SIZE];
extern super_block_t* super_block;

int main(void)
{
	//Initialize in memory hdd
	hdd_initialize();
	//Read file disk in memory
	hdd_disk_read(hdd);
	//Initialize buffer freelist and hashqueue
	p_freelist = create_list();
	initialize_freelist(p_freelist);

	p_hashqueue = create_hashqueue();

	//Initialize inode freelist and hashqueue
	i_p_freelist = i_create_list();
	i_initialize_freelist(i_p_freelist);

	i_p_hashqueue = i_create_hashqueue();

	//Create in memory dilb
	dilb_initialize();

	//Initialize in memory dilb with disk
	dilb_disk_read(dilb);

	//Initialize Superblock
	superblk_initialize();

	//Read disk contents of superblock
	superblk_disk_read(super_block);

	/*int mode = READ;
	int permissions[9] = {1,0,0,0,0,0,0,0,0};
	int fd = open("abc.txt",mode,permissions);

	char* buffer = (char*)malloc(15);

	//char* buffer;
	Read(fd,buffer,15);


	free(buffer);
	buffer = NULL;*/

	int mode = WRITE;
	int permissions[9] = {1,0,0,0,0,0,0,0,0};
	//int fd = open("abc.txt",mode,permissions);
	int fd1 = open("mno.txt",mode,permissions);
	//int fd2 = open("cde.txt",mode,permissions);

	char* buffer = (char*)malloc(33);
	strcpy(buffer,"gogotohellgogotohellgogotohell!!!");

	//char* buffer2 = (char*)malloc(20);
	//strcpy(buffer2,"helloworldgotohell");

	Write(fd1,buffer,33);
	//Write(fd1,buffer,10);

	char* buffer_read = NULL;
	Read(fd1,&buffer_read,33);

	printf("\n\nDATA READ  =  %s\n\n",buffer_read);

	i_show_iit(incore_inode_table,"\nIIT\n");
	i_show_ft(ft,"\nFT\n");
	i_show_ufdt(ufdt,"\nUFDT\n");

	print_superblk(super_block);

	i_show_freelist(i_p_freelist,"\nInode freelist\n");
	i_show_hashqueue(i_p_hashqueue,"Inode hashqueue\n");

	show_freelist(p_freelist,"\nBuffer Freelist\n");
	show_hashqueue(p_hashqueue,"\nBuffer hashqueue\n");

	//Write modified superblock to disk
	superblk_disk_write(super_block);

	//Destroy superblock
	destroy_superblk(super_block);

	//Write dilb to disk
	dilb_disk_write(dilb);

	//Destroy buffer freelist and hashqueue
	destroy_freelist(p_freelist);
	destroy_hashqueue(p_hashqueue);
	
	//Destroy inode freelist and hashqueue
	i_destroy_freelist(&i_p_freelist);
	i_destroy_hashqueue(&i_p_hashqueue);

	//Destroy dilb
	destroy_dilb(dilb);

	//Write file disk in memory
	hdd_disk_write(hdd);

	//Destroy in memory hdd
	destroy_in_memory_hdd();

    free(buffer);
	buffer = NULL;

	//free(buffer2);
	//buffer2 = NULL;

	printf("\nCode executed successfully!!\n");
}