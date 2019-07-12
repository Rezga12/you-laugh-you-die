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
	//გადაეცემა დამაუნთებული დირექტორია და ფაილის სახელი
	chdir(argv[1]);

	char block[100000];
	memset(block,67,100000);

	FILE *fp = fopen(argv[2], "w+");
	for(int i=0;i<5000;i++){
		fprintf(fp, "%s\n", block);
	}
	
	fclose(fp);


	return 0;
}