#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inode.h"
#include "chache.h"
#include "directory.h"
#include "list.h"


//sem_t mutex; 

static struct list * list;

bool cmp_inode(long long a, void * b);

bool inode_create(long long hash,int length,bool is_dir, mode_t mode){

	struct inode_disk inode;
	inode.length = length;
	inode.mode = mode;

	inode.num_sectors = length / BLOCK_SECTOR_SIZE;
	inode.num_sectors += length % BLOCK_SECTOR_SIZE == 0 ? 0 : 1;
	for(int i=0;i<inode.num_sectors;i++){
		char block[BLOCK_SECTOR_SIZE];
		bzero(block,BLOCK_SECTOR_SIZE);
		mem_block_write(hash,i,block);
	}
	inode.is_dir = is_dir;

	return mem_set_inode(hash,&inode);

}


void inode_init(){

	list = malloc(sizeof(struct list));
	list_init(list);

}


struct inode * inode_open(long long hash){

	//printf("hash: %lld\n",hash);
	long long a = hash;
	
	struct inode * inode_found = list_find(list,a,cmp_inode);
	if(inode_found != NULL){
		//free(inode);
		inode_found->open_cnt ++;
		//printf("found\n");
		return inode_found;
	}

	struct inode * node = malloc(sizeof(struct inode));
	
	if(!mem_get_inode(hash,&node->data)){
		return NULL;
	}
	node->hash = hash;
	node->open_cnt = 1;
  	node->pos = 0;
  	node->deny_write_cnt = 0;
  	node->removed = false;

  	list_add(list,node);

  	return node;
}

void inode_remove(struct inode *inode){
	mem_inode_remove(inode->hash);

  	for(int i=0;i<inode->data.num_sectors;i++){
		mem_block_remove(inode->hash,i);
	}

	list_remove (list,inode->hash,cmp_inode);

}

void inode_close (struct inode *inode){
  /* Ignore null pointer. */
  if (inode == NULL)
    return;
	//printf("%d\n",(int)inode->open_cnt);
  /* Release resources if this was the last opener. */
  	if (--inode->open_cnt == 0){
		/* Remove from inode list and release lock. */
		list_remove (list,inode->hash,cmp_inode);

		/* Deallocate blocks if removed. */
		/*if (inode->removed){
		  	mem_inode_remove(inode->hash);

		  	for(int i=0;i<inode->data.num_sectors;i++){
				mem_block_remove(inode->hash,i);
			}
		}*/

		

		//free (inode);
	}
}

struct inode * find_inode(long long hash){
	
	return list_find(list,hash,cmp_inode);
}





bool cmp_inode(long long a, void * b){
	//printf("%d\n",123);
	
	int sector_b = ((struct inode*)b)->hash;

	return sector_b == a;
}


size_t inode_read_at(struct inode *inode, void *buffer_, off_t size, off_t offset){

	if(offset >= inode->data.length){
		return 0;
	}

	off_t read_b = 0;

	while(size > 0){
		
		off_t starting_sector = offset / BLOCK_SECTOR_SIZE;
		off_t sector_offs = offset % BLOCK_SECTOR_SIZE;
		
		off_t inode_left = inode->data.length - offset;
      	int sector_left = BLOCK_SECTOR_SIZE - sector_offs;
      	int min_left = inode_left < sector_left ? inode_left : sector_left;

      	int chunk_size = size < min_left ? size : min_left;
      	
      	if (chunk_size <= 0){
      		break;
      	}

      	if(sector_offs == 0 && chunk_size == BLOCK_SECTOR_SIZE){
      		mem_block_read(inode->hash,starting_sector,buffer_ + read_b);
      	}else{
      		char buffer[BLOCK_SECTOR_SIZE] = {0};
      		mem_block_read(inode->hash,starting_sector,buffer);
      		memcpy(buffer_ + read_b,buffer + sector_offs, chunk_size);
      	}

      	

      	size -= chunk_size;
      	offset += chunk_size;
      	read_b += chunk_size;

	}
	
	return read_b;


}


off_t inode_write_at (struct inode *inode, const void *buffer_, off_t size,
                off_t offset)
{ 

	if (inode->deny_write_cnt)
    	return 0;

	if(offset + size > inode->data.length){
	    int total_grow = offset + size;
	    total_grow  -= (int)inode->data.length;
	    int sec_left = BLOCK_SECTOR_SIZE - (int)inode->data.length % BLOCK_SECTOR_SIZE;

	    inode->data.length += total_grow;
	}

	int bytes_w = 0;

	while (size > 0)
    {
		/* Sector to write, starting byte offset within sector. */
		int sector_idx = offset / BLOCK_SECTOR_SIZE;
		int sector_ofs = offset % BLOCK_SECTOR_SIZE;

		/* Bytes left in inode, bytes left in sector, lesser of the two. */
		off_t inode_left = inode->data.length - offset;
		int sector_left = BLOCK_SECTOR_SIZE - sector_ofs;
		int min_left = inode_left < sector_left ? inode_left : sector_left;

		/* Number of bytes to actually write into this sector. */
		int chunk_size = size < min_left ? size : min_left;
		if (chunk_size <= 0){
			break;
		}

      

		if(sector_ofs == 0 && chunk_size == BLOCK_SECTOR_SIZE){
			
			mem_block_write(inode->hash,sector_idx, (char*)buffer_ + bytes_w);
		}else{
			char buff[BLOCK_SECTOR_SIZE] = {0};
        	mem_block_read (inode->hash, sector_idx, buff);

        
      
			memcpy (buff + sector_ofs, buffer_ + bytes_w, chunk_size);
		
			mem_block_write(inode->hash,sector_idx,buff);
		}


      	

		if(sector_idx >= inode->data.num_sectors){
        	inode->data.num_sectors++;
        }

		/* Advance. */
		size -= chunk_size;
		offset += chunk_size;
		bytes_w += chunk_size;
    }

    mem_set_inode(inode->hash,&inode->data);

    return bytes_w;
}


int inodes_num(){

	return list_size(list);

}