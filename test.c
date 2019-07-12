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
  mode_t t;
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
      /*if (!mkdir (dir_name))
       {
          CHECK (remove (file_name), "remove \"%s\"", file_name);
          break;
        }*/

      /* Check for file and directory. */
      /*CHECK ((fd = open (".")) > 1, "open \".\"");
      CHECK (readdir (fd, name[0]), "readdir \".\"");
      CHECK (readdir (fd, name[1]), "readdir \".\"");
      CHECK (!readdir (fd, name[2]), "readdir \".\" (should fail)");
      CHECK ((!strcmp (name[0], dir_name) && !strcmp (name[1], file_name))
             || (!strcmp (name[1], dir_name) && !strcmp (name[0], file_name)),
             "names should be \"%s\" and \"%s\", "
             "actually \"%s\" and \"%s\"",
             file_name, dir_name, name[0], name[1]);
      close (fd);*/

      /* Descend into directory. */
      //CHECK (chdir (dir_name), "chdir \"%s\"", dir_name);
    //}
 /*CHECK (i > 200, "created files and directories only to level %d", i);
  quiet = false;

  msg ("removing all but top 10 levels of files and directories...");
  quiet = true;
  while (i-- > 10)
    {
      char file_name[16], dir_name[16];

      snprintf (file_name, sizeof file_name, "file%d", i);
      snprintf (dir_name, sizeof dir_name, "dir%d", i);
      CHECK (chdir (".."), "chdir \"..\"");
      CHECK (remove (dir_name), "remove \"%s\"", dir_name);
      CHECK (remove (file_name), "remove \"%s\"", file_name);
    }
  quiet = false;*/
}