#ifndef MEM_FILE_H
#define MEM_FILE_H

#include "inode.h"


struct file{
	struct inode *inode;        /* File's inode. */
    off_t pos;                  /* Current position. */
    bool deny_write;
};


bool file_create(long long hash,mode_t mode);
struct file * file_open(struct inode * inode);




#endif