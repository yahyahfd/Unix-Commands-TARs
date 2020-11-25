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

    //on place la tete de lecture au debut du fichier
    if (lseek(fd, 0, SEEK_SET) == (off_t) (-1)){
        RC_ERROR("Unable to seek file: %s", strerror(errno));
    }

    struct posix_header p;
    char filename = split(filepath,2);
    int lus,size=0;
    int static i=0;

    while(lus=read(fd,&p,sizeof(struct posix_header))>0){
      i+=512;
      if(cmp(filename,p.name)==0){
         if(p.typeflag=='5' && opt==1){
           perror("You cannot remove a non-empty file without the -r option");
           exit(EXIT_FAILURE);
           }
          lus=read(fd,&p,sizeof(struct posix_header));
          if(diff(filename,p.name)==1 && strlen(filename)==strlen(p.name)){
              i+=512;
           }
            if(diff(filename,p.name)==-1 && strlen(filename)!=strlen(p.name)){
            perror("This directory is not empty");
            exit(EXIT_FAILURE);
          }
          else{
            lseek(fd,i,SEEK_SET);
            i-=512;
            while(lus=read(fd,&p,sizeof(struct posix_header))>0){
              lseek(fd,i,SEEK_SET);
              i+=512;
              write(fd,&p,sizeof(struct posix_header));
              p.name[0]='\0';
                sscanf(p.size,"%o",&size);
              while(size>0){
                read(fd,&p,sizeof(struct posix_header));
                lseek(fd,i,SEEK_SET);
                i+=size;
                size-=512;
                write(fd,&p,size);
                  }
                }
              exit(EXIT_SUCCESS);
            }
              if(p.typeflag!='5'){
                sscanf(p.size,"%o",&size);
                i+=size;
              }
              }
            lseek(fd,i,SEEK_SET);
            }
        exit(EXIT_FAILURE);
          }

exit(EXIT_FAILURE);

    return ret;
}

/*************** MAIN *****************/

int main(int argc, char **argv){
	int opt=1;
	//verifie option
	if(argv[1][0]=='-'){
    if(argv[1][1]=='r'){
			opt=2;
		}else{
      fprintf(stderr, "Error: Bad option: %c\n", argv[1][i]);
      return 0;
    }
  }

	for(int i=opt;i<argc;i++){
		//si le chemin du fichier contient un tarball on utilise la fonction tar_remove
		if(cmp(split(argv[i],1),"echec")==-1 && cmp(split(argv[i],2),"echec")==-1){
        tar_remove(split(argv[i],1),opt);
		}else{
		    //sinon on execute la commande cat usuelle
				int pid=fork();
				if(pid<0)
					return (EXIT_FAILURE);
					if(pid==0){
            if(opt==2){
              execlp("rm","rm",argv[i],NULL);// with -r option
            }else{
              execlp("rm","rm",argv[i],NULL);// without -r option
            }
					}
					waitpid(pid,NULL,0);
		}
	}

	return 0;

}
