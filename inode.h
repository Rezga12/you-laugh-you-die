#ifndef MEM_INODE_H
#define MEM_INODE_H

#include <stdbool.h>
#include <stdlib.h>

struct inode_disk{
    
    size_t length;                       /* File size in bytes. */
    int num_sectors;            
    bool is_dir;
    mode_t mode;
    uid_t uid;
    gid_t gid;

    bool is_hard;
    bool is_sym;
    long long real_hash;

    nlink_t   nlink;
};


struct inode
{ 
    size_t pos;
    long long hash;              /* Sector number of disk location. */
    int open_cnt;                       /* Number of openers. */
    bool removed;                       /* True if deleted, false otherwise. */
    int deny_write_cnt;                 /* 0: writes ok, >0: deny writes. */
    struct inode_disk data;             /* Inode content. */
};

bool inode_create(long long hash, int length, bool is_dir, mode_t mode, uid_t uid, gid_t gid);
void inode_init();
struct inode * inode_open(long long hash);
void inode_close (struct inode *inode);
struct inode * find_inode(long long hash);
void inode_remove (struct inode *inode);
size_t inode_read_at(struct inode *inode, void *buffer_, off_t size, off_t offset);

off_t inode_write_at (struct inode *inode, const void *buffer_, off_t size,
                off_t offset);

int inodes_num();
struct inode * inode_find(long long hash);
void inode_remove(struct inode *inode);

void inode_truncate(struct inode * inode);

#endif