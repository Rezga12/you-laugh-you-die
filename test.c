#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

void
main (int argc, char *argv[])
{
  int i;

  printf ("creating one level of directories...\n");
  //quiet = true;

  chdir (argv[1]);

  char path[100];
  // strcat(path,argv[1]);
  // strcat(path,"/");
  // strcat(path,"start");
  mode_t t = ACCESSPERMS;
  mkdir ("start",t);

  struct dirent *pDirent;
  DIR *pDir;

  


  
  printf ("deleting some of directories...\n");

  chdir ("start");
  for (i = 0;i<1000; i++)
    {
      char name[3][50 + 1];
      char file_name[16], dir_name[16];
      char contents[128];
      int fd;

     

      
      snprintf (dir_name, sizeof dir_name, "dir%d", i);
      mkdir(dir_name,t);
      printf("%s\n",dir_name);

    }
  
    printf ("again creating some of directories...\n");


  for (i = 0;i<1000; i+=2)
    {
      char dir_name[20];

     

      
      snprintf (dir_name, sizeof dir_name, "dir%d", i);
      rmdir(dir_name);

    }

    
  for (i = 100;i<200 ; i++)
    {
      char name[3][50 + 1];
      char file_name[16], dir_name[16];
      char contents[128];
      int fd;

     

      
      snprintf (dir_name, sizeof dir_name, "dir%d", i);
      mkdir(dir_name,t);

    }

  chdir ("..");
    pDir = opendir ("start");
    i = 0;
  while ((pDirent = readdir(pDir)) != NULL) {
            //printf ("[%s]\n", pDirent->d_name);
    i++;
        }
        if(i==552){
          printf("::PASS::\n");
        }else{
          printf("::FAIL::\n");
        }
  
  closedir (pDir);
      
}