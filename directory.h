#ifndef MEM_DIR_H
#define MEM_DIR_H

#include "inode.h"

#define ROOT_INUMBER 1
#define NAME_MAX 50
#define ENTRY_FOR_BLOCK 16

/* A directory. */
struct dir
  {
    struct inode *inode;                /* Backing store. */
    off_t pos;                          /* Current position. */
  };

/* A single directory entry. */
struct dir_entry
  {
    long long hash;        /* Sector number of header. */
    char name[NAME_MAX + 1];            /* Null terminated file name. */
    bool in_use;                        /* In use or free? */
  };

bool dir_create(long long hash, int entry_cnt, mode_t mode);
struct dir * dir_open (struct inode *inode);
bool dir_add (struct dir *dir, const char *name, long long hash);
bool dir_readdir (struct dir *dir, char * name);
bool dir_lookup (const struct dir *dir, const char *name,struct inode **inode);
bool dir_close(struct dir * dir);
bool dir_remove(struct dir * dir,char * name);
bool dir_try_remove(struct dir *dir, const char *name);

#endif