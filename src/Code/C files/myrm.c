#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>       
#include "functions.h"

/**returns 1 if the 2 strings are different and -1 if not**/
int diff(char *deb ,char *str){
	for(int i=0;i<strlen(deb);i++){
		if(str[i]!=deb[i])
			return 1;
		}
		return -1;
	}

/***remove a file ***/
int rm(char *filepath){
	struct posix_header p;
	int i=0,j=0,k=0,size,lus;
	int fd=open(split(filepath,1),O_RDWR);
	if(fd==-1 ){
		write(1,"rm : error while opening the file \n",strlen("rm : error while opening the file \n"));
		exit(-1);
		}
	if(cmp(split(filepath,1),filepath)==0){
		write(1,"rm :Cannot delete file :  this is a directory \n",strlen("rm :Cannot delete file :  this is a directory \n"));
		exit(-1);
	}
	lseek(fd,0,SEEK_SET);
	while(read(fd,&p,512)>0){
			i+=512;
			j+=512;
			if(cmp(split(filepath,2),p.name)==0){
							/*if(p.typeflag=='5'){
								write(1,"rm : fail :this is a directory \n",strlen("rm : fail :this is a directory \n"));
								exit(-1);
								}*/
							sscanf(p.size, "%o", &size);	
							if(size%512!=0){
								size = ((size + 512 - 1) / 512) * 512;
								}
							j+=size;
							lseek(fd,i+size,SEEK_SET);
							while(( p.name[0]!='\0' && (lus=read(fd,&p,512))>0 )){
								lseek(fd,i-512,SEEK_SET);
								i+=512;
								j+=512;
								write(fd,&p,lus);
								lseek(fd,i+size,SEEK_SET);
								}
								lseek(fd,j-size-512,SEEK_SET);
								memset(&p,'\0',512);
								for(int i=0;i<(size+512)/512;i++){
									write(fd,&p,512);
									}
								set_checksum(&p);
								close(fd);
								return 0;
							}
					}
		close(fd);	
		write(1,"rm :Cannot delete file :  No such file or director5y \n",strlen("rm :Cannot delete file :  No such file or directory \n"));
		return 0;
	}
/***remove files and directories recursivly***/
int rmr(char *filepath){
	struct stat st;
	struct stat *buf = &st;
	struct posix_header p;
	int i=0;
	int fd=open(split(filepath,1),O_RDWR);
	if(fd==-1 ){
		write(1,"rm : 2No such file or directory \n",strlen("rm : 2No such file or directory \n"));
		return -1;
		}
	//on veut supprimer un tar
	if(cmp(split(filepath,1),filepath)==0){
		if(unlink(filepath)==-1){
			write(1,"rm : unlik error :Cannot delete file\n",strlen("rm : unlik error :Cannot delete file\n"));
			return -1;
			}
		return 0;
		}
	//on veut supprimer un dossier dans un tar
	lseek(fd,0,SEEK_SET);
	while(read(fd,&p,512)>0){
			i+=512;
			if(cmp(split(filepath,2),p.name)==0){
				char *fullname = strdup(split(filepath,1));
				while(diff(split(filepath,2),p.name)==-1 && p.name[0]!='\0'){
						
						strcat(fullname,"/");
						strcat(fullname,p.name);
						rm(fullname);
						free(fullname);
						lseek(fd,i-512,SEEK_SET);
						read(fd,&p,512);
				}
			free(fullname);
			return 0;
		}
	}
	write(1,"rm : No such file or directory ",strlen("rm : No such file or directory "));
	return -1;
}

int main(int argc,char **argv){
	int j=1,option=-1,n=0;
	
	if(argc<=1){
		 perror("rm : missing operand ");
		 exit(-1);
		}
	
	//verifie si l'un des arguments est une option
	while(j<argc && option!=0){
		if(argv[j][0]=='-' ){
			option=1;
			if(argv[j][1]=='r' || argv[j][1]=='R'){
				option=0;
				n+=j;
			}
					
					}
			j++;
		}
	if(option==1){
		execvp("/bin/rm",argv);
		}		

		for(int i=1;i<argc;i++){
			if(i==n){
				}
			else{
				if(cmp(split(argv[i],1),"echec")==-1 ){
					if(option==0){
							rmr(argv[i]);
						}
						else{
							if(argv[i][strlen(argv[i])-1]=='/'){
								write(1,"rm : this is a directory \n",strlen("rm : this is a directory \n"));
								}
							else{
							rm(argv[i]);
							}
						}
					}
						else{
							//sinon on execute la commande rm usuelle
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
