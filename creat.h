#define PERMISSION_ERROR -1

enum file_mode
{
	READ = 0,
	WRITE = 1 
};

#ifndef INODECACHE_H
#define INODECACHE_H

#include "inodecache.h"

#endif

#ifndef DILB_H
#define DILB_H

#include "dilb.h"

#endif

#ifndef CREAT_H
#define CREAT_H
int file_creation_mode;
int file_permissions[9];
#endif

int creat(char* file_name,int mode);