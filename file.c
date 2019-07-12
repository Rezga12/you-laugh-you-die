#include "chache.h"
#include "directory.h"
#include "list.h"
#include "file.h"

bool file_create(long long hash,mode_t mode){

	return inode_create(hash, 0, false, mode);

}

struct file * file_open(struct inode * inode){
	
	if(inode == NULL){
  		return NULL;
  	}

	struct file * file = malloc(sizeof(struct file));

	file->inode = inode;
	file->pos = 0;
	file->deny_write = false;

	return file;
}

