#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "tar.h"
#include <string.h>
#include <stdlib.h>


/***********compare deux chaînes de caractéres**************/

int cmp(char *name1,char *name2){
	for(int i=0;i<strlen(name1);i++){
		if(name1[i]!=name2[i])
			return -1;
		}
	return 0;
	}
/************récupére le contenu d'un fichier tar********/

int contenu_tar(char *chemin,char *filename){
	struct posix_header p;
	char buffer[512];
	char tmp[1024];
	int fd=open(chemin,O_RDONLY),j,i=0,lus;
	if(fd==-1)
		perror("le repertoire n'existe pas");
		
	do {
		lus=read(fd,&p,512);
	}	
	while(cmp(p.name,filename)==-1 && lus>0);
		
	if(cmp(p.name,filename)==0){
			if(((atoi(p.size)-512+1)/512) <1 )
			{
				lus=read(fd,tmp,atoi(p.size));
				write(STDOUT_FILENO ,tmp,lus);
			  }
			else
			{	
				while(j<((atoi(p.size)-512+1)/512)-2){	
					lus=read(fd,buffer,512);
					write(STDOUT_FILENO ,buffer,lus);
					j++;
					}
				}
		 close(fd);
		 
		 return 0;
		 
			}
	close(fd);
	return -1;

}
/***************vérifie si c'est une archive tar********************/

int isTar (char c[]){
   int i = strlen (c) - 3;
   if ((c[i] == 't') && (c[i + 1] == 'a') && (c[i + 2] == 'r')){
      return 1;
   }
   return 0;
}
			
			
		
