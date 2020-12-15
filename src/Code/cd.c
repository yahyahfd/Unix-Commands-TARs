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
char *path_behind(char *filepath){
	char *res=strdup(filepath);
	for(int i=strlen(res)-2;i>0;i--){
		if(res[i]=='/'){
			res[i]='\0';
			return res;
			}
	}
	return filepath;
}	
/********************************/	
char *cd(char *filepath,char *wd){
	char *pwd;
	struct stat st;
	struct stat *buf = &st;
	struct posix_header p;
	const char* env = getenv("HOME");
	char tmp[MAX_LEN];
	//pas d'arguments
	if(filepath==NULL){
		chdir(env);
		pwd=strdup(env);
	}
	/*****************************************************************************/
	//si on fait cd .. pour retourner en dossier pere
	if(cmp(filepath,"..")==0){
		pwd=strdup(cd(path_behind(wd),pwd));
		return pwd;
				}
				
	if(cmp(filepath,"/")==0){
		chdir("/");
		pwd=strdup("/");
		return pwd;
				}
	//si le chemin ne contient pas un tar 
	if(cmp(split(filepath,1),"echec")==0){
		if(chdir(filepath)==-1){
			//le dossier n'existe pas
			if(stat(filepath,buf)<0){
				write(1,"cd : No such file or directory\n",strlen("cd : No such file or directory\n"));
			}
			//le fichier n'est pas un dossier
			else{
				if((buf->st_mode & S_IFMT)!=S_IFDIR){
					write(1,"cd : Not a directory\n",strlen("cd : Not a directory\n"));
			}
		}
	}	
		getcwd(tmp,MAX_LEN);
		pwd=strdup(tmp);	
		return pwd;	
	}
	/****tar part****/
	else{
		//si on fait cd dans un tar
		if(cmp(split(filepath,2),"")==0){
			int fd = open(split(filepath,1),O_RDONLY);
			if(fd==-1){
				write(1,"cd : No such file or directory\n",strlen("cd : No such file or directory\n"));
				chdir(before_tar(filepath));
				getcwd(tmp,MAX_LEN);
				pwd=strdup(tmp);
				return pwd;
				}
			chdir(before_tar(filepath));
			getcwd(tmp,MAX_LEN);
			pwd=strdup(tmp);
			strcat(pwd,"/");
			strcat(pwd,tar_name(filepath));
			return pwd;
		}
		else{
			//si on fait cd dans un dossier dans un tar
			int fd =open(split(filepath,1),O_RDONLY);
			if(fd==-1 ){
				write(1,"cd: error while opening the file\n",strlen("cd: error while opening the file\n"));
				exit(-1);
			}
			int b=0;
			while(read(fd,&p,512)>0){
				if(cmp(split(filepath,2),p.name)==0){
					//le fichier n'est pas un dossier
					if(p.typeflag!='5'){
						write(1,"cd : Not a directory\n",strlen("cd : Not a directory\n"));
						chdir(before_tar(split(filepath,1)));
						getcwd(tmp,MAX_LEN);
						pwd=strdup(tmp);
						strcat(pwd,"/");
						strcat(pwd,tar_name(filepath));	
						return pwd;
						}
					b=1;
					}
				}
			//le dossier n'existe pas dans le tar
			if(b==0){
				write(1,"cd : No such file or directory\n",strlen("cd : No such file or directory\n"));
				chdir(before_tar(split(filepath,1)));
				getcwd(tmp,MAX_LEN);
				pwd=strdup(tmp);
				strcat(pwd,"/");
				strcat(pwd,tar_name(filepath));
				return pwd;
			}
			//le dossier existe dans le tar
			chdir(before_tar(split(filepath,1)));
			getcwd(tmp,MAX_LEN);
			pwd=strdup(tmp);
			strcat(pwd,"/");
			strcat(pwd,tar_name(filepath));		
			strcat(pwd,"/");		
			strcat(pwd,split(filepath,2));		
				}
			}
	
	return pwd;
}



int main(int argc,char **argv){
	char *pwg=cd(argv[1],"");printf("%s\n",pwg);
	char *c= cd(argv[2],pwg);printf(" %s\n",c);
	return 0;
	}
