#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>  
#include <time.h>
#include <pwd.h>
#include <grp.h>     
#include "functions.h"    
#include <readline/readline.h>
#include <readline/history.h>
#include "rm.h"
#include "cd.h"
#include "cat.h"
#include "rmdir.h"
#include "cp.h"
#include "reds.h"
//#include "ls.h"
#define MAX_LEN 250
	

char **split_line(char *line){
	
	int pos=0,count=nb_words(line);

	char **tokens = malloc(count*sizeof(char*));
	char *token;
	if(tokens==NULL){
		perror("fail of malloc");
		exit(EXIT_FAILURE);
		}
	token =strtok(line," ");
	while(token!=NULL){
		tokens[pos]=token;
		pos++;
		token=strtok(NULL," ");
		
		}
	tokens[pos]=NULL;
	return tokens;
	
	}


int execute_command(int argc,char **commands){
	if (argc == 0) {
		return 1;
	}
		pid_t pid;
		int status;
		pid = fork();
		if (pid == 0) {
		
		/**********************RM**************************/

		if(cmp(commands[0],"rm")==0){
			tsh_rm(argc,commands);
		}
		
		/**********************EXIT**************************/

		else if(cmp(commands[0],"exit")==0){
			kill(pid,SIGSTOP);
		}
		/**********************CAT**************************/
		
		else if(cmp(commands[0],"cat")==0){
			tsh_cat(argc,commands);			
		}
		/**********************RMDIR**************************/
		
		else if(cmp(commands[0],"rmdir")==0){
			tsh_rmdir(argc,commands);
		}
		/**********************CP**************************/
		
		else if(cmp(commands[0],"cp")==0){
			tsh_cp(argc,commands);
		}
		/**********************LS**************************/
		
		else{
			if(execvp(commands[0],commands)==-1){
				perror("tsh");
				}
				return 0;
			
			}
		/*if(cmp(commands[0],"ls")==0){
			tsh_ls(argc,commands);
		}*/
		
		
	}
	else if (pid < 0) {
		return -1;
			} 
				
	  else {

		do {
		  waitpid(pid, &status, WUNTRACED);
		} 
		while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	
	return 1;

}


	
int main(){
	char hostname[50];
	gethostname(hostname,50);
	char *line;
	char **commands;
	int status,argc=0;
	char tmp[MAX_LEN];
	char *c=malloc(strlen("tsh @ ")+strlen(hostname)+strlen(" : "));
	strncat(c,"tsh @ ",strlen("tsh @ "));
	strncat(c,hostname,strlen(hostname));
	strncat(c," : ",strlen(" : "));
	const char* env = getenv("HOME");
	char *pwd=strdup(getcwd(tmp,MAX_LEN));
	do {
		line=readline(c);
		argc=nb_words(line);
		commands=split_line(line);
		int j=0,k=0;
		int n=0;
		/************************************redirections sortie*************/
		for(int i=0;i<argc;i++){
			if(cmp(commands[i],">")==0 || cmp(commands[i],">>")==0 ){
				   n+=i;
			}
		}
		
		if(n!=0){
			char **args=malloc((argc-1)*sizeof(char *));
			char *tmp;
			char *file=strdup(commands[n+1]);
				 while(k<argc){ 
					 if(k!=n && k!=(n+1) && j<(argc-1) && commands[k]!=NULL){
						 tmp=strdup(commands[k]);
						 args[j]=tmp;
						 j++;
						 k++;
					}
					else{
						k++;
						}
					}
			
			if(cmp(split(file,1),"echec")==0 && cmp(commands[n],">")==0){

					redirection_sn(file);
					
				 }
			 else if(cmp(split(file,1),"echec")==0 && cmp(commands[n],">>")==0){
				 redirection2_sn(file);
				 }
			 else if(cmp(split(file,1),"echec")!=0 && cmp(commands[n],">")==0){
				 redirection_tar_file(file);
				 }
			 else if(cmp(split(file,1),"echec")!=0 && cmp(commands[n],">>")==0){
				 redirection2_tar_file(file);
				 }
				
			commands=malloc((argc-1)*sizeof(char *));
			for(int i=0;i<argc-2;i++){
				 tmp=strdup(args[i]);
				 commands[i]=tmp;
				 
				}
			argc=argc-2;
			free(args);
			free(file);
			}
		/**********************CD**************************/
		
		if(cmp(commands[0],"cd")==0){
				//pas d'arguments
			
			if(argc==1){
				chdir(env);
				pwd=strdup(env);
			}
			else if(argc<2){
				pwd=strdup(env);
				}
			else if(argc==2){
				pwd=strdup(cd(commands[1],pwd));
				}
			else{
				write(1,"cd : too many arguments\n",strlen("cd : too many arguments\n"));
				}
			}
		/**********************PWD**************************/
		
		else if(cmp(commands[0],"pwd")==0){
			
			write(1,pwd,strlen(pwd));
			write(1,"\n",strlen("\n"));
			}
			
		else{
			status=execute_command(argc,commands);			
					
		}
		
		free(line);
		free(commands);
	} 
		while (status);
		free(c);
	return 0;
	
}

