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


/**********affiche le contenu du fichier filename **********/
int contenu_tar(char *chemin,char *filename){
	struct posix_header p;
	char buffer[512];
	int fd=open(chemin,O_RDONLY),j,i=0,lus,size;
	if(fd==-1){
		perror("Cannot open file");
		exit(EXIT_FAILURE);
	}
		do {
			lus=read(fd,&p,512);
		}	
		while(cmp(p.name,filename)==-1 && lus>0);
			
		if(cmp(p.name,filename)==0){
			sscanf(p.size,"%o",&size);
			for(int i=0;i<((size+512-1)/512);i++){
				lus=read(fd,buffer,512);
				write(STDOUT_FILENO,buffer,lus);
			}	
				close(fd);
			 return 0;
			 
				}
		close(fd);
		perror("No such file");
		return -1;
}


/***************la fonction cat*****************/
	
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
			//si le chemin du fichier contient un tarball on utilise la fonction contenu_tar
			if(cmp(split(argv[i],1),"echec")==-1 ){
					if (strlen(argv[i])!=strlen(split(argv[i],1)))
					{
						contenu_tar(split(argv[i],1),split(argv[i],2));
						}
					else {
						perror("Cat : This is a directory");
						exit(-1);
					}
				}
					else{
						//sinon on execute la commande cat usuelle
						int pid=fork();
						if(pid<0)
							return (EXIT_FAILURE);
						if(pid==0){
								execlp("cat","cat",argv[i],NULL);
							}		
							waitpid(pid,NULL,0);
							}
						}	
		}
	return 0;

}

