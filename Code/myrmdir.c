#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "functions.h"	

int diff(char *deb ,char *str){
	for(int i=0;i<strlen(deb);i++){
		if(str[i]!=deb[i])
			return 1;
		}
		return -1;
	}
int remove_dir(char *filepath){
	struct stat st;
	struct stat *buf = &st;
	
	if(cmp(split(filepath,2),"echec")==0)
	{
		stat(filepath,buf);
			if(buf->st_size==1024){
				int c=unlink(filepath);
				exit(EXIT_SUCCESS);
			}
			else{
			perror("this directory is not empty");
			exit(EXIT_FAILURE);
		}
		perror("this is not a directory");	
		exit(EXIT_FAILURE);
			}
	else
	    {
			struct posix_header p;
			int fd=open(split(filepath,1),O_RDWR);
			int lus,size=0;
			int static i=0;
			if(fd==-1){
				perror("Erreur while opening the file");
				exit(EXIT_FAILURE);
			}
			while(lus=read(fd,&p,sizeof(struct posix_header))>0){
				i+=512;
				if(cmp(split(filepath,2),p.name)==0){
					 if(p.typeflag!='5'){
						 perror("This is not a directory");
						 exit(EXIT_FAILURE);
						 }
						lus=read(fd,&p,sizeof(struct posix_header));
						if(diff(split(filepath,2),p.name)==1 && strlen(split(filepath,2))==strlen(p.name)){
								i+=512;
						 }
						  if(diff(split(filepath,2),p.name)==-1 && strlen(split(filepath,2))!=strlen(p.name)){
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
}

int main(int argc, char **argv){
	remove_dir(	argv[1]);
	return 0;
}
