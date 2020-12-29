#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "tar.h"
#include <string.h>
#include <stdlib.h>


/**returns 1 if the 2 strings are different and -1 if not**/
int diff(char *deb ,char *str){
	for(int i=0;i<strlen(deb);i++){
		if(str[i]!=deb[i])
			return 1;
		}
		return -1;
	}

/**retourne le nombre de mots d'une chaîne**/
int nb_words(char *line){
	int count=1;
	for(int i=0;line[i]!='\0';i++){
		if(line[i]==' ' || line[i] == '\t' || line[i] == '\n')
		{
			count++;
			}
		
		}
		return count;
	}

/**verifie si le chemin contient un tar ,si oui elle renvoie le chemin du tar et celui du fichier  *******/
char *split(char *path,int num){
	char *tar=strdup(path),*filename=strdup("");
	int i=0,n=0;
	int f=fork();
	if(f==-1){
		perror("fork error");
		exit(-1);
		}
	wait(NULL);
	if(f==0){
		if(strlen(path)<5){
			 return "echec";
			}
		while(i!=-1 && i<strlen(path)-3){
				if(path[i]=='.' && path[i+1]=='t' && path[i+2]=='a' && path[i+3]=='r'){
					tar[i+4]='\0';
					i=-1;
					n=-1;
				}
				else{
					i++;
				}
			}
			if(n==0){
				return "echec";
			}
			//filename=malloc(strlen(path)-strlen(tar));
			strncat(filename,path+strlen(tar)+1,strlen(path)-strlen(tar));
		if(num==1)
			return tar;
		if(num==2)
			return filename;
		return "";
	}
	free(tar);
	free(filename);
	exit(1);
	}
	

/***********compare deux chaînes de caractéres**************/

int cmp(char *name1,char *name2){
	if(strlen(name1)!=strlen(name2))
		return -1;
	for(int i=0;i<strlen(name1);i++){
		if(name1[i]!=name2[i])
			return -1;
		}
	return 0;
	}
/***************vérifie si c'est une archive tar********************/

int isTar (char *filepath){
   for(int i=0;i<strlen(filepath)-3;i++){
		if(filepath[i]=='.' && filepath[i+1]=='t' && filepath[i+2]=='a' &&filepath[i+3]=='r' )
				return 1;
				}
	return 0;
	}
/*************************************************************/
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
/*******************************************************************/
char *path_behind1(char *filepath){
	char *res=strdup(filepath);
	for(int i=strlen(res)-2;i>0;i--){
		if(res[i]=='/'){
			res[i+1]='\0';
			return res;
			}
	}
	return "";
}				

