#define FILE_DOES_NOT_EXISTS -1
#define FILE_PERMISSIONS_NOT_VALID -2

typedef int file_descriptor_t;

#ifndef INODECACHE_H
#define INODECACHE_H

#include "inodecache.h"

#endif

#ifndef CREAT_H
#define CREAT_H

#include "creat.h"

#endif

file_descriptor_t open(char* file_name,int mode);

