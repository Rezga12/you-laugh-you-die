#include <string.h>
#include "chache.h"
#include "directory.h"
#include "list.h"
#include "file.h"

bool file_create(long long hash,mode_t mode, uid_t uid, gid_t gid){

	return inode_create(hash, 0, false,mode, uid, gid);

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



bool symlink_create(long long hash, mode_t mode, uid_t uid, gid_t gid, const char * path){

	inode_create(hash, 0, false,mode, uid, gid);

	struct inode * inode = inode_open(hash);

	inode->data.is_sym = true;

	mem_set_inode(hash,&inode->data);

	inode_write_at(inode, path,strlen(path),0);



	return true;
}


