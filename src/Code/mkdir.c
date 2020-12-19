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

char *split_bis(char *path,int num){
  char *existing_dir=strdup(path),*new_dir=NULL;
  int tmp;

  for (size_t i = 0; path[i]!='\0'; i++) {
    if (path[i]== '/') {
      if(path[i+1]!= '\0')
      tmp = i;
    }
  }
  existing_dir[tmp]='\0';
  new_dir=malloc(strlen(path)-strlen(existing_dir));
  strncat(new_dir,path+strlen(existing_dir)+1,strlen(path)-strlen(existing_dir));

  if (num = 1) return existing_dir;
  if (num = 2) return new_dir;

  free(existing_dir);
  free(new_dir);
}

int dir_create(char *filepath, char *new_dir){
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
  int lus;
  int i=0,j=0;

  while(read(fd,&p,512)>0 ){
    i+=512;
    j+=512;
    if (p.name[0] == '\0') {
      lseek(fd,512,SEEK_CUR);
      write(fd,&p,512);
      strcpy(p.name,new_dir);
      p.typeflag ='5';
      lseek(fd,i-512,SEEK_SET);
      write(fd,&p,512);
      set_checksum(&p);
      exit(0);
    }
  }
  return -1;
}

int main(int argc, char **argv){
  int i=1;
  if(argc<=1){
		 perror("mkdir : missing operand ");
		 exit(-1);
	}
  fprintf(stderr, "%s\n",split(argv[1],1) );
  fprintf(stderr, "%s\n", split(argv[1],2));

  fprintf(stderr, "%s\n",split_bis(split(argv[1],2),1));
  fprintf(stderr, "%s\n",split_bis(split(argv[1],2),2));

  for(int i=1;i<argc;i++){
    	if(cmp(split(argv[i],1),"echec")==-1){
        dir_create(split(argv[i],1),"New");
      }else{
        int pid=fork();
        if(pid<0)
          return (EXIT_FAILURE);
        if(pid==0){
          execlp("mkdir","mkdir",argv[i],NULL);
        }
        waitpid(pid,NULL,0);
      }
  }

}
