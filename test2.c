#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

//int ii = 0;
int rec_count(char * dirn){
	struct dirent *pDirent;
  DIR *pDir;
  //printf("%d\n",ii++);
  pDir = opendir (dirn);
    int i = 0;
    chdir(dirn);
   while ((pDirent = readdir(pDir)) != NULL) {
            
   		if(!strcmp(".",pDirent->d_name) || !strcmp("..",pDirent->d_name)){
   			continue;
   		}
   		//printf ("[%s]\n", pDirent->d_name);
   		i += rec_count(pDirent->d_name);

   }
   chdir("..");

   return i + 1;
}

void rec_del(char * dirn){

	struct dirent *pDirent;
  DIR *pDir;
  //printf("%d\n",ii++);
  pDir = opendir (dirn);
    int i = 0;
    chdir(dirn);
    char*a[10];
    //int i = 0;
   while ((pDirent = readdir(pDir)) != NULL) {
            //printf ("[%s]\n", pDirent->d_name);
   		if(!strcmp(".",pDirent->d_name) || !strcmp("..",pDirent->d_name)){
   			continue;
   		}
   		//rec_del(pDirent->d_name);
   		a[i] = pDirent->d_name;
   		i++;

   }

   for(int j=0;j<i;j++){
   	rec_del(a[j]);
   }


   chdir("..");
   rmdir(dirn);

   


}

void rec_create_two_files(int depth){
	if(depth == 0){
		return;
	}

	mode_t t;
  	mkdir ("first",t);
  	chdir("first");
  	rec_create_two_files(depth - 1);
  	chdir("..");

  	mkdir ("second",t);
  	chdir("second");
  	rec_create_two_files(depth - 1);
  	chdir("..");


}

void rec_create_one_files(int depth){
	if(depth == 0){
		return;
	}

	mode_t t;
  	mkdir ("a",t);
  	chdir("a");
  	rec_create_two_files(depth - 1);
  	chdir("..");

  	mkdir ("b",t);
  	chdir("b");
  	rec_create_two_files(depth - 1);
  	chdir("..");


}


void
main (int argc, char *argv[])
{	

	printf("recursively creating bunch of files...\n");

	chdir(argv[1]);

	mode_t t;
	mkdir ("root",t);
	chdir("root");


	rec_create_two_files(7);
	chdir("..");

	printf("recursively counting number of those files...\n");

	int n = rec_count("root");
	if(n == 255){
		//printf("::PASS::\n");
	}else{
		printf("FAIL: number of files is %d must be 512.\n",n);
		//return;
	}

	printf("recursively deleting number of those files...\n");

	chdir("root");
	rec_del("second");
	chdir("..");

	n = rec_count("root");

	if(n == 128){
		//printf("::PASS::\n");
	}else{
		printf("FAIL: number of files after deletion is %d must be 128.\n",n);
		
	}

/*
	printf("creating more new files...\n");
	chdir("root");
	rec_create_one_files(7);
	chdir("..");

	n = rec_count("root");
	if(n == 382){
		printf("::PASS::\n");
	}else{
		printf("FAIL: number of files is %d must be 382.\n",n);
		//return;
	}*/



	//printf("%d\n",n);
	

}