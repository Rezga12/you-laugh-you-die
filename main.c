/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.
*/

/** @file
 *
 * minimal example filesystem using high-level API
 *
 * Compile with:
 *
 *     gcc -Wall hello.c `pkg-config fuse3 --cflags --libs` -o hello
 *
 * ## Source code ##
 * \include hello.c
 */


#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <sys/vfs.h>
#include <unistd.h>
#include "chache.h"
#include "directory.h"
#include "file.h"
#include <pthread.h> 
#include <semaphore.h> 
/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */

/*
static struct options {
	const char *filename;
	const char *contents;
	int show_help;
} options;

#define OPTION(t, p)                           \
    { t, offsetof(struct options, p), 1 }
static const struct fuse_opt option_spec[] = {
	OPTION("--name=%s", filename),
	OPTION("--contents=%s", contents),
	OPTION("-h", show_help),
	OPTION("--help", show_help),
	FUSE_OPT_END
};

*/


   	
static sem_t mutex; 
static sem_t getattr;
static void *hello_init(struct fuse_conn_info *conn,
			struct fuse_config *cfg)
{	
	
	sem_init(&mutex, 0, 1); 
	sem_init(&getattr,0,1);
	
	(void) conn;
	cfg->kernel_cache = 1;
	return NULL;
}

static int hello_getattr(const char *path, struct stat *st,
			 struct fuse_file_info *fi)
{	
	//sem_wait(&getattr);
	
	
	printf("hola %s\n",path);
	(void) fi;
	int res = 0;

	

	memset(st, 0, sizeof(struct stat));
	
	//
	struct inode * inode = inode_open(get_hash(path));
	
	

	if(inode == NULL){
		//ASSERT(0);
		//printf("ret\n");
		//sem_post(&getattr);
		return -ENOENT;
	}

	printf("mode: %d\n",inode->data.mode);

	printf("getattr: %s,, hash: %lld\n",path,get_hash(path));
	printf("return hash: %lld\n",inode->hash);

	

		st->st_dev = 0;         /* ID of device containing file */
               st->st_ino = inode->hash;         /* Inode number */
               st->st_mode = inode->data.mode;        /* File type and mode */
               st->st_nlink = 1;       /* Number of hard links */
               uid_t     st_uid;         /* User ID of owner */
               gid_t     st_gid;         /* Group ID of owner */
               dev_t     st_rdev;        /* Device ID (if special file) */
               st->st_size = inode->data.length;        /* Total size, in bytes */
               st->st_blksize = BLOCK_SECTOR_SIZE;     /* Block size for filesystem I/O */
               st->st_blocks = inode->data.num_sectors * 2;      /* Number of 512B blocks allocated */

	if(inode->data.is_dir){
		st->st_mode = S_IFDIR | st->st_mode;
	}else {
		st->st_mode = S_IFREG | st->st_mode;
	}



	//FILE *fp = fopen("/home/rezga/Desktop/assignment/text.txt", "a");
	//fprintf(fp, "tried to getattr: %s\n", path);
	//fclose(fp);

	
	/*
	if (strcmp(path, "/") == 0) {
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 1;
		st->st_size = inode->data.length;
	
	}else {
		if(inode->data.is_dir){
			st->st_mode = S_IFDIR | 0755;
			st->st_nlink = 1;
			st->st_size = inode->data.length;
		}else{
			st->st_mode = S_IFREG | 0444;
			st->st_nlink = 1;
			st->st_size = inode->data.length;
		}
		
		
	}*/
	// }else{
		
	// 	return -ENOENT;
	// }
	//printf("-getattr: %s\n",path);
	
	//sem_post(&getattr);
	return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi,
			 enum fuse_readdir_flags flags)
{	//printf("red\n");
	(void) offset;
	(void) fi;
	(void) flags;
	//sem_wait(&getattr);
	//FILE *fp = fopen("/home/rezga/Desktop/assignment/text.txt", "a");
	//fprintf(fp, "tried to read: %s\n", path);
	//fclose(fp);

	struct dir * dir = dir_open (inode_open(get_hash(path)));

	if (dir == NULL)
		//sem_post(&getattr);
		return -ENOENT;
	//dir->pos = offset;
	//printf("%d\n",(int)fi->fh);

	filler(buf, ".", NULL, 0, 0);
	filler(buf, "..", NULL, 0, 0);
	
	//printf("%d\n",(int)offset);
	char name[50] = {0};
	while(dir_readdir (dir, name)){
	//if(dir_readdir (dir, name)){
		filler(buf, name, NULL, 0, 0);
	}
	//printf("%d\n",(int)dir->pos);
		
	//}
	dir_close(dir);
	//sem_post(&getattr);
	return 0;
}

static int memcache_open(const char *path, struct fuse_file_info *fi)
{	
	//printf("open %s\n",path);
	//struct file * file = file_open(inode_open(get_hash(path)));
	long long hash = get_hash(path);
	inode_open(hash);
	(void) fi;
	fi->fh = (int)hash;

	/*if (strcmp(path+1, "facebook") != 0)
		return -ENOENT;

	if ((fi->flags & O_ACCMODE) != O_RDONLY)
		return -EACCES;*/


	return 0;
}


bool get_without_last(const char * path, char * buff, char * name){

	int len = 0;
	for(int i=0;path[i] != 0;i++){
		if(path[i] == '/'){
			len = i;
		}
	}

	strncpy(buff,path,len);

	strcpy(name,path + len + 1);


	return len == 0;
}

static int memcache_mkdir(const char * path, mode_t mode){


	//printf("create: %s\n",path);

	//printf("mode from mkdir: %d\n", mode);

	long long hash = get_hash(path);
	long long parent_hash;

	char without_last[20480];
	bzero(without_last,20480);
	char name[55];
	bzero(name,55);

	if(get_without_last(path, without_last,name)){
		parent_hash = get_hash("/");
	}else{
		parent_hash = get_hash(without_last);
	}

	//printf("create %s in %s\n",name,without_last);
	
	//printf("1\n");
	dir_create(hash,ENTRY_FOR_BLOCK,mode);
	//printf("2\n");
	struct dir * dir = dir_open (inode_open(parent_hash));
	//printf("3\n");
	printf("mkdir hash: %lld\n",hash);
	ASSERT(dir_add(dir,name, hash));
	//printf("4\n");



	return 0;

}

static int memcache_opendir(const char * path, struct fuse_file_info * fi){
	//FILE *fp = fopen("/home/rezga/Desktop/assignment/text.txt", "a");
	//fprintf(fp, "tried to open: %s\n", name);
	//fclose(fp);
	struct dir * dir = dir_open (inode_open(get_hash(path)));
	fi->fh = dir->inode->hash;

	//printf("opendir\n");

	return 0;

}


static int memcache_releasedir(const char * path, struct fuse_file_info *fi){

	//printf("releasedir\n");

	struct inode * inode_found = find_inode(fi->fh);
	if(inode_found != NULL){
		inode_close(inode_found);
	}

	return 0;

}

static int memcache_unlink(const char * path){
	struct dir * dir = dir_open (inode_open(get_hash(path)));
	
	if(dir == NULL){
		return -ENOENT;
	}
	
	dir->inode->removed = true;
	inode_remove(dir->inode);
	dir_close(dir);

	char without_last[20480];
	char name[55];

	bzero(without_last,20480);
	bzero(name,55);

	struct dir * parent;

	if(get_without_last(path, without_last, name)){
		parent = dir_open (inode_open(get_hash("/")));
	}else{
		parent = dir_open (inode_open(get_hash(without_last)));
	}
	
	//printf("%lld\n",get_hash(without_last));

	//printf("parent to remove: %s\n", without_last);
	//printf("name to remove: %s\n", name);

	ASSERT(dir_try_remove(parent,name));

	dir_close(parent);

	return 0;
}

static int memcache_rmdir(const char * path){

	//ENOTEMPTY


	struct dir * dir = dir_open (inode_open(get_hash(path)));
	if(dir_readdir(dir,NULL)){
		return -ENOTEMPTY;
	}
	printf("%d\n",dir->inode->open_cnt);
	dir->inode->removed = true;
	inode_remove(dir->inode);
	dir_close(dir);

	

	char without_last[20480];
	char name[55];

	bzero(without_last,20480);
	bzero(name,55);

	struct dir * parent;

	if(get_without_last(path, without_last, name)){
		parent = dir_open (inode_open(get_hash("/")));
	}else{
		parent = dir_open (inode_open(get_hash(without_last)));
	}
	
	//printf("%lld\n",get_hash(without_last));

	//printf("parent to remove: %s\n", without_last);
	//printf("name to remove: %s\n", name);

	ASSERT(dir_try_remove(parent,name));

	dir_close(parent);

	return 0;

}




static int memcache_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{	


	sem_wait(&mutex); 
	printf("<read size: %d, off: %d, path: %s\n",(int)size,(int)offset,path);
	long long hash = fi->fh;
	
	(void) fi;
	struct inode * inode =inode_open(get_hash(path));
	//printf("%lld\n",get_hash(path));
	
	if(inode == NULL){
		//struct inode * inode = inode_open(get_hash(path));
		//fi->fh = get_hash(path);
		sem_post(&mutex); 
		return -ENOENT;
	}

	if(inode->data.is_dir){
		//printf("woops:(\n");
		inode_close(inode);
		sem_post(&mutex); 
		return -EISDIR;
	}

	int a = (int)inode_read_at(inode,buf,size,offset);
	//inode_close(inode);
	printf(">read size: %d, off: %d\n",(int)size,(int)offset);
	sem_post(&mutex); 
	return a;

	
}



static int memcache_write (const char * path, const char *buff, size_t size, off_t offset, 
				struct fuse_file_info *fi)
{
	//printf("%d\n",++a);
	//printf("size: %d, ofset: %d \n",(int)size,(int)offset);
	printf("%d\n",fi->flags);

	(void) fi;
	struct inode * inode = inode_open(get_hash(path));
	
	if(inode == NULL)
		return -ENOENT;

	if(inode->data.is_dir)
		return -EISDIR;

	if(fi->flags & O_APPEND){
		//inode->data.length = size + offset;
		offset = inode->data.length;
	}
	printf("write size: %d, off: %d\n",(int)size,(int)offset);
	//printf("%s\n",buff);

	int a = inode_write_at(inode,buff,size,offset);
	inode_close(inode);
	return a;
}



static int memcache_statfs (const char * path, struct statvfs * buff){

	buff->f_bsize = BLOCK_SECTOR_SIZE;
	buff->f_frsize = BLOCK_SECTOR_SIZE;
	buff->f_files = inodes_num();
	return 0;
}

static void memcache_destroy(void *private_data){
	mem_destroy();
	
}

static int memcache_create(const char * path, mode_t mode, struct fuse_file_info * fi){

	long long hash = get_hash(path);
	long long parent_hash;

	printf("creating %s\n",path);

	char without_last[20480];
	bzero(without_last,20480);

	char name[55];
	bzero(name,55);

	if(get_without_last(path, without_last,name)){
		parent_hash = get_hash("/");
	}else{
		parent_hash = get_hash(without_last);
	}

	//printf("create file %s in /%s\n",name,without_last);

	struct dir * parent = dir_open(inode_open(parent_hash));
	
	ASSERT(file_create(hash,mode));
	
	ASSERT(dir_add(parent,name,hash));

	return 0;
}




static int memcache_release(const char * path, struct fuse_file_info * fi){
	(void) fi;
	//printf("closed %s\n",path);
	return 0;


}


//TODO override fsynch dir

static int memcache_chown(const char * path, uid_t uid, gid_t gid, struct fuse_file_info *fi){

	printf("chown: %u\n", uid);
	printf("chown: %u\n", gid);

	return -EACCES;

}


static int memcache_utimens(const char * path, const struct timespec tv[2], struct fuse_file_info *fi){
	return 0;
}

static int memcache_read_buf(const char * path, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info * fi){

	printf("fkin readbuf? %d,%d\n",(int)size, (int)off);


	return 0;
}

static int memcache_access(const char * path, int mode){

}






static struct fuse_operations hello_oper = {
	.init       = hello_init,
	.getattr	= hello_getattr,
	.readdir	= hello_readdir,
	.open		= memcache_open,
	.release    = memcache_release,
	.unlink		= memcache_unlink,
	.read		= memcache_read,
	.write      = memcache_write,
	.statfs     = memcache_statfs,
	.mkdir      = memcache_mkdir,
	.opendir    = memcache_opendir,
	.releasedir = memcache_releasedir,
	.rmdir      = memcache_rmdir,
	.create     = memcache_create,
	.utimens    = memcache_utimens,
	.chown = memcache_chown,
	//.read_buf   =memcache_read_buf,

};





/*
static void show_help(const char *progname)
{
	printf("usage: %s [options] <mountpoint>\n\n", progname);
	printf("File-system specific options:\n"
	       "    --name=<s>          Name of the \"hello\" file\n"
	       "                        (default: \"hello\")\n"
	       "    --contents=<s>      Contents \"hello\" file\n"
	       "                        (default \"Hello, World!\\n\")\n"
	       "\n");
}
*/
int main(int argc, char *argv[])
{	
	//TODO
	

	//struct dir * dir = dir_open (inode_open(ROOT_INUMBER));
	//dir_lookup(dir,"path+1",NULL);
	//dir_close(dir);

	char ** my_argv = malloc(sizeof(char*) * (argc+1));
	for(int i=0;i<=argc;i++){
		my_argv[i] = argv[i];
	}

	my_argv[argc] = "-s";

	mem_init();
	// if(argc == 4){
	 	mem_clear();
	struct inode_disk inode;
	if(!mem_get_inode(get_hash("/"),&inode)){

	 	dir_create(get_hash("/"), ENTRY_FOR_BLOCK, 0);
	}
	// }
	
	//mem_set_icounter(3);
	//printf("%d\n",ENTRY_FOR_BLOCK);
	
	inode_init();

	//printf("%d\n",(int)sizeof(struct dir_entry)*ENTRY_FOR_BLOCK);
	//struct dir * dir = dir_open (inode_open(ROOT_INUMBER));

	int ret;
	struct fuse_args args = FUSE_ARGS_INIT(argc+1, my_argv);

	/* Set defaults -- we have to use strdup so that
	   fuse_opt_parse can free the defaults if other
	   values are specified */
	//options.filename = strdup("hello");
	//options.contents = strdup("Hello World!\n");

	/* Parse options */
	//if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1)
	//	return 1;

	/* When --help is specified, first print our own file-system
	   specific help text, then signal fuse_main to show
	   additional help (by adding `--help` to the options again)
	   without usage: line (by setting argv[0] to the empty
	   string) */
	/*if (options.show_help) {
		show_help(argv[0]);
		assert(fuse_opt_add_arg(&args, "--help") == 0);
		args.argv[0][0] = '\0';
	}*/

	//printf("%d\n",(int)sizeof(size_t));

	printf("%d\n",S_IFDIR | 0755);

	ret = fuse_main(args.argc, args.argv, &hello_oper, NULL);
	fuse_opt_free_args(&args);

	free(my_argv);
	return ret;
}
