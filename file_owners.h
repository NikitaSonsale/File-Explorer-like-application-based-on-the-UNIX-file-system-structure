#ifndef FILEOWNERS_H
#define FILEOWNERS_H

int remembered_file_owner_number;

#endif

int check_if_file_owner_exits(char* file_owner);
int get_file_owner_number(char* file_owner);
void write_file_owner(char* file_owner,int file_owner_number);