#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

int ls(char *dir){
  DIR *dirp;
  struct dirent *entry;
	struct stat sb;

	if(stat(dir, &sb)==-1){
		perror("stat");
		exit(1);
	}

	if(S_ISDIR(sb.st_mode)){ //dir is a directory
	  if((dirp = opendir(dir)) == NULL) {//opening direcotory
	    fprintf(stderr, "Impossible d'ouvrir le répertoire '%s'.\n", dir);
	    perror("Erreur ");
	    exit(EXIT_FAILURE);
	  }
	  while((entry=readdir(dirp))){//printing each file/folder name inside dir
	    printf("%s\n", entry->d_name);
	  }
		if(closedir(dirp) == -1) {//closing directory
			perror("Erreur lors de la fermeture du repertoire");
			exit(EXIT_FAILURE);
		}
	}else{ //dir is a file
		printf("%s\n", dir);
	}
}

int main(int argc, char **argv){
	if(argc==1){ //"ls" called alone
		ls(".");
	}else if(argc==2){ //"ls -l" or "ls directory_path" or "ls file_path"
		if((strcmp(argv[1], "-l") == 0)){

		}else{
			ls(argv[1]);
		}
	}else if(argc==3){ //"ls -l +  directory_path or file_path"
		if((strcmp(argv[1], "-l") == 0)){

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
