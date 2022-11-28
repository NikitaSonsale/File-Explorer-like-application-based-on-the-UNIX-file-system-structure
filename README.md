# File-Explorer-like-application-based-on-the-UNIX-file-system-structure

we have created a file explorer like application based on the unix file system structure using which user can create new files, close the
files, read data from file and write on a file. We have implemented free list of buffers using which we can allocate a free buffer for disk block. 
A buffer can be present on the free list only if it is unlocked. The order or sequence of buffers on freelist is maintained by "Least Recently Used" 
algorithm. we have implemented the hash queue for searching a specific buffer in the list. Also we have implemented algorithms related to file subsystem 
such as getblk algorithm which allocates requested buffer from the buffer pool to the respective block, brelease algorithm to free the buffer, bread 
algorithm to read the disk block and bwrite algorithm to write buffer contents to disk.  
