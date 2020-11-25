#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "tar.h"
#include <string.h>
#include <stdlib.h>

/********split*****************/
/**verifie si le chemin contient un tar ,si oui elle renvoie le chemin du tar et celui du fichier  *******/
char *split(char *path,int num){
	char *tar=strdup(path),*filename=NULL;
	int i=0,n=0;
	int f=fork();
	if(f==-1){
		perror("fork error");
		exit(-1);
		}
	wait(NULL);
	if(f==0){
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
			if(n==0)
				return "echec";
			filename=malloc(strlen(path)-strlen(tar));
			strncat(filename,path+strlen(tar)+1,strlen(path)-strlen(tar)-1);
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

	/***********since "cmp" didn't take into account that "path" is similar to "path/" **************/
	int samepath(char *name1,char *name2){
		if((strlen(name1)==strlen(name2)+1)){
			for(int i=0;i<strlen(name2);i++){
				if(name1[i]!=name2[i])
					return -1;
			}
			if(name1[strlen(name2)=='/'])
				return 0;
		}
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
