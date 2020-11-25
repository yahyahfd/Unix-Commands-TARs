#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "functions.h"

int tar_remove(char *filepath,int opt){

  char filename = split(filepath,2);

  struct posix_header p;
  int fd=open(split(filepath,1),O_RDWR);
  if(fd==-1){
    perror("Cannot open this file");
    exit(EXIT_FAILURE);
  }

    // get file permissions
    struct stat st;
    if (fstat(fd, &st)){
        RC_ERROR("Unable to stat archive: %s", strerror(errno)); //obtenir le libellé d'un numero d'erreur
    }

    // reset offset of original file
    if (lseek(fd, 0, SEEK_SET) == (off_t) (-1)){    //on place la tete de lecture au debut du fichier
        RC_ERROR("Unable to seek file: %s", strerror(errno));
    }

    // find first file to be removed that does not exist
    int ret = 0;

    unsigned int read_offset = 0;
    unsigned int write_offset = 0;
    struct tar_t * prev = NULL;
    struct tar_t * curr = *p;

    

    // resize file
    if (ftruncate(fd, write_offset) < 0){
        RC_ERROR("Could not truncate file: %s", strerror(rc));
    }

    // add end data
    if (write_end_data(fd, write_offset) < 0){
        V_PRINT(stderr, "Error: Could not close file");
    }

    return ret;
}

/*************** MAIN *****************/

int main(int argc, char **argv){
	int i=1,option=-1;
	//verifie si l'un des arguments est une option
	while(i<argc){
		if(argv[i][0]=='-' ){
				option=0;
					}
			i++;
		}
	if(option==0){
		execvp("/bin/cat",argv);
	}

	else{

		for(int i=1;i<argc;i++){
			//si le chemin du fichier contient un tarball on utilise la fonction tar_remove
			if(cmp(split(argv[i],1),"echec")==-1 && cmp(split(argv[i],2),"echec")==-1)
					{

						tar_remove(split(argv[i],1),split(argv[i],2));
						}
					else{
						//sinon on execute la commande cat usuelle
						int pid=fork();
						if(pid<0)
							return (EXIT_FAILURE);
						if(pid==0){
								execlp("rm","rm",argv[i],NULL);
							}
							waitpid(pid,NULL,0);
							}
						}
		}
	return 0;

}
