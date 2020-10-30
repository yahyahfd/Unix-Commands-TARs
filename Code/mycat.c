#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "functions.h"


/**********affiche le contenu du fichier filename **********/
int contenu_tar(char *chemin,char *filename){
	struct posix_header p;
	char buffer[512];
	char tmp[1024];
	int fd=open(chemin,O_RDONLY),j,i=0,lus;
	if(fd==-1){
		perror("le repertoire n'existe pas");
		exit(EXIT_FAILURE);
	}
		do {
			lus=read(fd,&p,512);
		}	
		while(cmp(p.name,filename)==-1 && lus>0);
			
		if(cmp(p.name,filename)==0){
			printf("%s\n",p.name);
			printf("%s\n",filename);
			printf("le size %d\n",(atoi(p.size)));
			printf("%d\n",((atoi(p.size)-512+1)/512));

				if(((atoi(p.size)-512+1)/512)<1)
				{
					printf("%d\n",(atoi(p.size)));
					lus=read(fd,tmp,atoi(p.size));
					write(STDOUT_FILENO ,tmp,lus);
				  }
				else
				{	
					while(j<((atoi(p.size)-512+1)/512)-1 && lus>0){	
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
/********split*****************/
/**verifie si le chemin contient un tar ,si oui elle renvoie le chemin du tar et celui du fichier  *******/
char *split(char *path,int num){
	char *tar=strdup(path),*filename=NULL;
	int i=0,n;
	while(i!=-1){
			if(path[i]=='.' && path[i+1]=='t' && path[i+2]=='a' && path[i+3]=='r'){
				tar[i+4]='\0';
				i=-1;
			}
			else{
				i++;
			}
		}
		if(strlen(path)-strlen(tar)==0)
			return "echec";
		filename=malloc(strlen(path)-strlen(tar));
		strncat(filename,path+strlen(tar)+1,strlen(path)-strlen(tar)-1);
	if(num==1)
		return tar;
	if(num==2)
		return filename;
	if(strlen(path)-strlen(tar)==0)
		return "echec";
	return "";
	}	


/***************la fonction cat*****************/
	
int main(int argc, char **argv){

	int fd;
	int lus;
	char buffer[1000];
	
	if(argc<2){
		while(lus =read(0, buffer ,sizeof(buffer)))
			write(1 ,buffer ,lus);
	}
	else{
			
		for(int i=1;i<argc;i++){
			
			if(cmp(split(argv[i],1),"echec")==-1 && cmp(split(argv[i],2),"echec")==-1)
			{
				contenu_tar(split(argv[i],1),split(argv[i],2));
				}
				
			if(cmp(split(argv[i],1),"echec")==0)
			{
				fd=open(argv[i], O_RDONLY);
			
				if(fd == -1)
					perror("Cannot open file");
					
				else{
					
					while(lus =read(fd, buffer ,sizeof(buffer)))
						write(STDOUT_FILENO ,buffer ,lus);
						
					close(fd);
					}
			}
		}
		
	}
	return 0;

}
