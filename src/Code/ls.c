#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "functions.h"

int ls(char *dir, int isl){
  DIR *dirp;
  struct dirent *entry;
  const char *rights[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-","rwx", NULL };
  int right;
  int b;
	struct stat sb;

	if(stat(dir, &sb)==-1){
		perror("stat");
		exit(1);
	}

	if(S_ISDIR(sb.st_mode)){ //dir is a directory
	  if((dirp = opendir(dir)) == NULL) {//opening direcotory
	    perror("Error while opening the directory ");
	    exit(EXIT_FAILURE);
	  }
	  while((entry=readdir(dirp))){//printing each file/folder name inside dir
      if(isl==0){
        printf("%s\n", entry->d_name);
      }else{
        b = lstat (entry->d_name, &sb);
				if (b == -1){
          printf ("Inexistant path\n");
        }else {
          printf ("%s", rights[(right>>6)&7] ); //Owner
          printf ("%s", rights[(right>>3)&7] ); //Group
          printf ("%s", rights[(right)&7] ); //Others
          printf (" %s\n",entry->d_name);
          right = sb.st_mode;
        }
	    }
    }
		if(closedir(dirp) == -1) {//closing directory
			perror("Error while closing the directory");
			exit(EXIT_FAILURE);
		}
	}else{ //dir is a file
    if(isTar(dir)){
      struct posix_header p;
      int fd=open(dir,O_RDONLY),i=0,lus;

      while(lus>0){
        lus=read(fd,&p,512);
        printf("%s\n", p.name);
        int x = (atoi(p.size)-512+1)/512;
        do {
          lus=read(fd,&p,512);
          i++;
        }
        while(i<x);
      }
      if(close(fd) == -1) {//closing directory
        perror("Error while closing the tarball ");
        exit(EXIT_FAILURE);
      }
    }else{
  		printf("%s\n", dir);
    }
	}
}

int main(int argc, char **argv){
	if(argc==1){ //"ls" called alone
		ls(".",0);
	}else if(argc==2){ //"ls -l" or "ls directory_path" or "ls file_path" or "ls tar"
		if((cmp(argv[1], "-l") == 0)){
      ls(".",1);
		}else{
			ls(argv[1],0);
		}
	}else if(argc==3){ //"ls -l +  directory_path or file_path"
		if((cmp(argv[1], "-l") == 0)){
      ls(argv[2],1);
		}else{ //syntax error
			fprintf(stderr, "Erreur de syntaxe.\n");
			exit(EXIT_FAILURE);
		}
	}else{ //syntax error
		fprintf(stderr, "Erreur de syntaxe.\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}
