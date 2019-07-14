#ifndef MEM_FILE_H
#define MEM_FILE_H

#include "inode.h"


struct file{
	struct inode *inode;        /* File's inode. */
    off_t pos;                  /* Current position. */
    bool deny_write;
};


bool file_create(long long hash,mode_t mode, uid_t uid, gid_t gid);
struct file * file_open(struct inode * inode);
bool symlink_create(long long hash,mode_t mode, uid_t uid, gid_t gid, const char * path);



#endif