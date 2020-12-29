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
#define MAX_LEN 250
/***retourne le nom du tar **/
char *tar_name(char *filepath){
	int nb=0;char *lus;
	char *tmp=strdup(split(filepath,1));
	char *tmp2=strdup(split(filepath,1));
	lus=strtok(tmp,"/");
	if(lus==NULL){
		nb=0;
	}
	else{
		nb++;
		while( strtok(NULL,"/")!=NULL){
			nb++;
			}
		}
	if(nb==1){
		return filepath;
		}
	char *name=strtok(tmp2,"/");
	for(int i=0;i<nb-1;i++){
		name=strtok(NULL,"/");

		}
		return name;
	}
/**retourne le chemin avant le tar**/
char *before_tar(char *filepath){
	int j=0;
	char *res=strdup(split(filepath,1));
	for(int i=0;i<strlen(res)-2;i++){
		if(filepath[i]=='.' && filepath[i+1]=='t' && filepath[i+2]=='a' &&filepath[i+3]=='r' ){
				j=i;
			}
				}
	for(j;j>0;j--){
		if(res[j]=='/'){
			res[j]='\0';
			return res;
			}
	}
	return res;
	}
/***retourne dans le dossier d'avant**/	
char *path_behind1(char *filepath){
	char *res=strdup(filepath);
	for(int i=strlen(res)-2;i>0;i--){
		if(res[i]=='/'){
			res[i+1]='\0';
			return res;
			}
	}
	return filepath;
}	
#define MAX_LEN 250


/**returns the path before the tar directory**/
char *before_tar(char *filepath){
	int j=0;
	char *res=strdup(split(filepath,1));
	for(int i=0;i<strlen(res)-2;i++){
		if(filepath[i]=='.' && filepath[i+1]=='t' && filepath[i+2]=='a' &&filepath[i+3]=='r' ){
				j=i;
			}
				}
	for(j;j>0;j--){
		if(res[j]=='/'){
			res[j]='\0';
			return res;
			}
	}
	return res;
	}
/***returns to the directory before**/	
char *path_behind(char *filepath){
	char *res=strdup(filepath);
	for(int i=strlen(res)-2;i>0;i--){
		if(res[i]=='/'){
			res[i+1]='\0';
			return res;
			}
	}
	return filepath;
}	
/************cd function********************/	
char *cd(char *filepath,char *wd){
	char *pwd;
	struct stat st;
	struct stat *buf = &st;
	struct posix_header p;
	const char* env = getenv("HOME");
	char tmp[MAX_LEN];

	//if we do cd .. to get back to the directory before
	if(cmp(filepath,"..")==0){
		pwd=strdup(cd(path_behind(wd),path_behind(wd)));
		return pwd;
				}
				
	if(cmp(filepath,"/")==0){
		chdir("/");
		pwd=strdup("/");
		return pwd;
				}
	//the path doesn't contain a tar
	if(cmp(split(filepath,1),"echec")==0){
		//si on est pas deja dans un tar
		if(cmp(split(wd,1),"echec")==0){
			if(chdir(filepath)==-1){
				//the directory doesnt exist
				if(stat(filepath,buf)<0){
					write(1,"cd : No such file or directory\n",strlen("cd : No such file or directory\n"));
					return wd;
				}
				//the file is not a directory
				else{
					
					if((buf->st_mode & S_IFMT)!=S_IFDIR){
						write(1,"cd : Not a directory\n",strlen("cd : Not a directory\n"));
						return wd;
							}
					}
			}		
			getcwd(tmp,MAX_LEN);
			pwd=strdup(tmp);	
			return pwd;	
		}
		if(cmp(split(wd,1),"echec")==-1){
			//if we are inside the tar and the path starts with a /
			if(filepath[0]=='/'){
				pwd=strdup(cd(filepath,pwd));
				return pwd;
			}
			
			if(filepath[0]=='.' && filepath[1]=='.' && filepath[2]=='/'){
				//if we are inside a tar and the path starts with ../
				char *n=strdup("");;
				strcat(n,path_behind(wd));
				strcat(n,filepath+3);
				pwd=strdup(cd(n,pwd));
				return pwd;
				}
				
			else{
				//si on est deja dans un tar et on saisie le nom d'un dossier dans le tar(sans reecrire t.tar/....)
				chdir(before_tar(wd));
				getcwd(tmp,MAX_LEN);
				char *n=strdup(tmp);
				strcat(n,"/");
				strcat(n,tar_name(wd));
				strcat(n,"/");
				strcat(n,filepath);
				pwd=strdup(cd(n,pwd));
				}
			}
	return pwd;
}
	/****tar part****/
	else{
		//if we do cd inside a tar directory
		if(cmp(split(filepath,2),"")==0){
			int fd = open(split(filepath,1),O_RDONLY);
			if(fd==-1){
				write(1,"cd : No such file or directory\n",strlen("cd : No such file or directory\n"));
				return wd;
				}
			chdir(before_tar(filepath));
			getcwd(tmp,MAX_LEN);
			pwd=strdup(tmp);
			strcat(pwd,"/");
			strcat(pwd,tar_name(filepath));
			return pwd;
		}
		else{
			//if we do cd to a directory inside a tar
			int fd =open(split(filepath,1),O_RDONLY);
			if(fd==-1 ){
				write(1,"cd: error while opening the file\n",strlen("cd: error while opening the file\n"));
				return wd;
			}
			int b=0;
			while(read(fd,&p,512)>0){
				if(cmp(split(filepath,2),p.name)==0){
					//the file is not a directory
					if(p.typeflag!='5'){
						write(1,"cd : Not a directory\n",strlen("cd : Not a directory\n"));	
						chdir(before_tar(filepath));
						getcwd(tmp,MAX_LEN);
						pwd=strdup(tmp);
						strcat(pwd,"/");
						strcat(pwd,tar_name(filepath));
						return pwd;
						}
					b=1;
					}
				}
			//directory doesn't exist inside the tar 
			if(b==0){
				write(1,"cd : No such file or directory\n",strlen("cd : No such file or directory\n"));
				chdir(before_tar(filepath));
				getcwd(tmp,MAX_LEN);
				pwd=strdup(tmp);
				strcat(pwd,"/");
				strcat(pwd,tar_name(filepath));
				return pwd;
			}
			//directory exist inside the tar
			chdir(before_tar(filepath));
			getcwd(tmp,MAX_LEN);
			pwd=strdup(tmp);
			strcat(pwd,"/");
			strcat(pwd,tar_name(filepath));	
			strcat(pwd,"/");
			strcat(pwd,split(filepath,2));
			return pwd;		
				}
			}
	
	return pwd;
} 

int main(int argc,char **argv){
	cd(argv[1]);
	return 0;
	}
