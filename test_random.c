#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <sys/vfs.h>
#include <unistd.h>

int main (int argc, char *argv[]){

	chdir(argv[1]);
	//printf("%d\n",(int)0x0400);
	int fd = fd = open(argv[2], O_RDWR|O_CREAT, 0777);
	char w[100] = "rezobezoylezo\n";
	pwrite(fd,w,strlen(w),100000);
	char a[100] = {0};
	pread(fd,a,10,100000);
	printf("%s\n",a);



	return 0;
}