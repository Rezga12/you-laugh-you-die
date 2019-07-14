#ifndef CACHE_DISK_H
#define CACHE_DISK_H

#include <stdbool.h>
#include <assert.h>
#include "inode.h"

#define PORT 11211
#define BLOCK_SECTOR_SIZE 1024
#define ASSERT assert

void mem_clear(void);
void mem_init(void);

void mem_destroy(void);
void mem_clear(void);

bool mem_set_icounter(int counter);
int mem_get_icounter();
int mem_inode_add(struct inode_disk * data);

bool mem_set_inode(long long hash, struct inode_disk * inode);
bool mem_get_inode(long long hash,struct inode_disk * inode);
bool mem_block_read(long long hash,int sector,void * block);
bool mem_block_write(long long hash,int sector,void * block);

bool mem_block_add(char * path,int sector);

bool mem_inode_remove(long long hash);
bool mem_block_remove(long long hash,int i);
long long get_hash(const char * path);


int mem_getxattr(long long hash, const char * mkey, char*value);
bool mem_setxattr(long long hash, const char * mkey, const char*value,size_t size);
//int mem_get_next_inumber();

#endif