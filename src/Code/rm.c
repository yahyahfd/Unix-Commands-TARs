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

int occurences(char *filepath){
	struct posix_header p;
	static int k;
	int fd=open(split(filepath,1),O_RDWR);
	if(fd==-1){
		perror("error while opening the file");
		exit(-1);
		}
	k=0;
	while(read(fd,&p,512)>0 && p.name[0]!='\0'){
		if(cmp(split(filepath,2),p.name)==0){
				k+=1;
				}
	}
		return k;

	}

int tar_remove(char *filepath,int opt){

  int fd=open(split(filepath,1),O_RDWR);
  if(fd==-1){
    perror("Cannot open this file");
    exit(EXIT_FAILURE);
  }

    // get file permissions
    struct stat st;
    if (fstat(fd, &st)){
        perror("Unable to stat archive");
        fprintf(stderr, "Unable to stat archive: %s\n", strerror( errno ));   //obtenir le libellé d'un numero d'erreur
    }

    //on place la tete de lecture au debut du fichier
    if (lseek(fd, 0, SEEK_SET) == (off_t) (-1)){
        perror("Unable to seek file");
        fprintf(stderr, "Unable to seek file: %s\n", strerror( errno ));
    }

  struct posix_header p;
  char *filename = split(filepath,2);
  int lus,size=0;
  int i=0,j=0;

  while(lus=read(fd,&p,sizeof(struct posix_header))>0){
    i+=512;
    j+=512;
    if(cmp(filename,p.name)==0){
      if(occurences(filepath)==0){
        write(STDOUT_FILENO, "rmdir: No such file or directory\n", strlen("rmdir: No such file or directory\n"));
       exit(EXIT_FAILURE);
      }
      if(occurences(filepath)>1 && opt == 1){
        write(STDOUT_FILENO, "rm: Cannot delete a non-empty directory without the -r option\n", strlen("rm: Cannot delete a non-empty directory without the -r option\n"));
       exit(EXIT_FAILURE);
     }else{
       lseek(fd,i,SEEK_SET);
       read(fd,&p,512);
       if(	p.name[0]=='\0'){
         memset(&p,'\0',512);
         lseek(fd,i-512,SEEK_SET);
         write(fd,&p,512);
         set_checksum(&p);
         exit(0);
       }else{
         lseek(fd,i,SEEK_SET);
         while((lus=read(fd,&p,512))>0 && p.name[0]!='\0'){
           lseek(fd,i-512,SEEK_SET);
           i+=512;
           write(fd,&p,lus);
           lseek(fd,512,SEEK_CUR);
           j+=512;
         }
         memset(&p,'\0',512);
         lseek(fd,j-512,SEEK_SET);
         write(fd,&p,512);
         set_checksum(&p);
         exit(0);
       }
     }

    }
    lseek(fd,i,SEEK_SET);
  }

  return 0;
}

/*************** MAIN *****************/

int main(int argc, char **argv){
	int opt=1;
	//verifie option
	if(argv[1][0]=='-'){
    if(argv[1][1]=='r'){
			opt=2;
		}else{
      fprintf(stderr, "Error: Bad option: %c\n", argv[1][1]);
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
