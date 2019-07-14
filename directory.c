#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "directory.h"
#include "inode.h"
#include "chache.h"



bool dir_create(long long hash, int entry_cnt, mode_t mode, uid_t uid, gid_t gid){
	//printf("%d\n",(int)sizeof(struct dir_entry)*entry_cnt);
	return inode_create(hash, (int)sizeof(struct dir_entry)*entry_cnt,true, mode,  uid,  gid);

}

struct dir * dir_open (struct inode *inode){
  	if(inode == NULL){
  		return NULL;
  	}
	
	struct dir *dir = malloc (sizeof(struct dir));
	dir->inode = inode;
	dir->pos = 0;
	return dir;
}

bool dir_add (struct dir *dir, const char *name, long long hash){
	struct dir_entry e;
	size_t ofs;
	bool success = false;

	/* Check NAME for validity. */
	if (*name == '\0' || strlen (name) > NAME_MAX){
		return false;
	}

	struct inode * inode = malloc(sizeof(struct inode));
	if (dir_lookup (dir, name, &inode)){
		printf("%s already exists\n",name);
		return false;
	}
    


	for(int i=0;i<dir->inode->data.num_sectors;i++){
		struct dir_entry block[ENTRY_FOR_BLOCK];
		mem_block_read(dir->inode->hash,i,block);

		for(int j=0;j<ENTRY_FOR_BLOCK;j++){
			struct dir_entry * cur = &block[j];
			if(!cur->in_use){
				cur->in_use = true;
				cur->hash = hash;
				//printf("asdasdsa\n");
				strcpy(cur->name,name);
				//printf("asdasdsa\n");
				mem_block_write(dir->inode->hash,i,block);
				return true;
			}
		}
	}
	struct dir_entry block[ENTRY_FOR_BLOCK];
	bzero(block,sizeof(struct dir_entry) * ENTRY_FOR_BLOCK);
	block[0].in_use = true;
	block[0].hash = hash;
	strcpy(block[0].name,name);
	dir->inode->data.length += BLOCK_SECTOR_SIZE;
	free(inode);

	int next_sector = dir->inode->data.num_sectors;
	dir->inode->data.num_sectors++;
	mem_set_inode(dir->inode->hash,&dir->inode->data);
    return mem_block_write(dir->inode->hash,next_sector,block);
	
  
}

bool dir_try_remove(struct dir *dir, const char *name){
	if(dir == NULL){
		printf("directory is null\n");
  		return false;
  	}
  	//int inode_sector = dir->inode->hash;

  	for(int i=0;i<dir->inode->data.num_sectors;i++){
		struct dir_entry block[ENTRY_FOR_BLOCK];
		ASSERT(mem_block_read(dir->inode->hash,i,block));

		for(int j=0;j<ENTRY_FOR_BLOCK;j++){
			if(block[j].in_use)
				printf("name: %s , %s\n",block[j].name,name);
			if(block[j].in_use && !strcmp(name,block[j].name)){

				block[j].in_use = false;
				ASSERT(mem_block_write(dir->inode->hash,i,block));
				return true;
			}

		}
	}
	printf("directory by name '%s' wasn't found in dir\n",name);
	//*inode = NULL;
	return false;
}

bool dir_lookup (const struct dir *dir, const char *name,struct inode **inode){
  	if(dir == NULL){
  		return false;
  	}
  	//int inode_sector = dir->inode->hash;

  	for(int i=0;i<dir->inode->data.num_sectors;i++){
		struct dir_entry block[ENTRY_FOR_BLOCK];
		//printf("%lld\n",dir->inode->hash);
		ASSERT(mem_block_read(dir->inode->hash,i,block));

		for(int j=0;j<ENTRY_FOR_BLOCK;j++){
			if(block[j].in_use && !strcmp(name,block[j].name)){
				if(inode != NULL)
					*inode = inode_open(block[j].hash);
				return true;
			}

		}
	}
	//*inode = NULL;
	return false;

}

bool dir_readdir (struct dir *dir, char * name){
  
	int inode_sector = dir->inode->hash;
	int starter = dir->pos % ENTRY_FOR_BLOCK;
	for(int i=dir->pos/ENTRY_FOR_BLOCK;i<dir->inode->data.num_sectors;i++){
		struct dir_entry block[ENTRY_FOR_BLOCK];
		mem_block_read(dir->inode->hash,i,block);

		for(int j=starter;j<ENTRY_FOR_BLOCK;j++){
			dir->pos++;
			if(block[j].in_use){
				if(name != NULL)
					strcpy(name,block[j].name);
				return true;
			}

		}
		starter = 0;
	}


  	return false;
}


bool dir_close(struct dir * dir){

	inode_close(dir->inode);
	free(dir);

	return true;
}



