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
#include "rm.h" 
#define MAX_SIZE  1024


char *path_behind(char *filepath){
	char *res=strdup(filepath);
	for(int i=strlen(res)-2;i>0;i--){
		if(res[i]=='/'){
			res[i+1]='\0';
			return res;
			}
	}
	return "";
}	

	/**************************returns the number of occurences of a file***************************************/
	int occurences(char *filepath){
		struct posix_header p;
		static int k;
		int fd=open(split(filepath,1),O_RDWR);
		if(fd==-1){
			perror("error while opening the file\n");
			exit(-1);
			}
		k=0;
		while(read(fd,&p,512)>0 && p.name[0]!='\0'){
			if(diff(split(filepath,2),p.name)==-1){
					k+=1;
					}
		}
			close(fd);
			return k;

		}
		/**********************************returns the position of end of file*************************************************/
	int file_end(int d){
	int end=0;
	struct posix_header p;
	lseek(d,0,SEEK_SET);
	while(read(d,&p,512)>0 && p.name[0]!='\0' ){
		end+=512;
		
		}
	return end;
	}
		/*********************returns the user and groupe name********************************/
	const char *uname(){
		
		uid_t uid = geteuid();
		struct passwd *pw = getpwuid(uid);
		if (pw){
			return pw->pw_name;
		}
		return "";
	}

	const char *gname(){
		gid_t gid = getegid();
		struct group *pw = getgrgid(gid);
		if (pw){
			return pw->gr_name;
		}
		return "";
	}

	/************** returns the file name without the path **************/
	char *file_name(char *filepath){
		int nb=0;char *lus;
		char *tmp=strdup(filepath);
		char *tmp2=strdup(filepath);
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
				name=strdup(strtok(NULL,"/"));

				}
				return name;
		}
		
/************************redirections to a normal file***************************************/
int redirection_sn(char *destination){
		if(opendir(destination)!=NULL){
		write(1, "tsh : destination file : this is a directory\n",strlen("tsh : destination file : this is a directory\n"));
		}
	if(cmp(path_behind1(destination),"")!=0 && opendir(path_behind1(destination))==NULL){
		
		write(1, "tsh : destination file : no such file or directory\n",strlen("tsh : destination file : no such file or directory\n"));

		}
	 
	 int d = open(destination,O_RDWR|O_CREAT|O_TRUNC);
	chmod (destination,S_IWUSR|S_IRUSR);
	 if(d<0){
		 perror("error while opening destination file ");
		 }
	dup2(d,1);
	close(d);
	}
/*********************redirections >> to a normal file***************/
	
int redirection2_sn(char *destination){
	
	struct stat st;
	struct stat *buf = &st;
	if(opendir(destination)!=NULL){
		write(1, "tsh : destination file : this is a directory\n",strlen("tsh : destination file : this is a directory\n"));
		}
	if(cmp(path_behind(destination),"")!=0 && opendir(path_behind(destination))==NULL){
		
		write(1, "tsh : destination file : no such file or directory\n",strlen("tsh : destination file : no such file or directory\n"));

		}
	//the file exists
	if(stat(destination,buf)==0){
		chmod (destination,S_IWUSR|S_IRUSR);
		 int d = open(destination,O_APPEND|O_WRONLY);
		 if(d<0){
				perror("error while opening destination file ");
		 }
		 dup2(d,1);	
		}
	//the file doesnt exist
	else{
		printf("yeees\n");
	 redirection_sn(destination);
	}
}



/***************redirections > to a tar file*************/

int redirection_tar_file(char *destination){
	struct posix_header hd;
	 int d = open(split(destination,1),O_RDWR);
	 int i=0,size=0;
	if(d<0){
		 perror("error while opening destination file ");
		 	 		return -1;

		 }
	 else if(cmp(split(destination,2),"")==0 || destination[strlen(destination)-1]=='/'){
		 write(1, "tsh : destination file : this is a directory\n",strlen("tsh : destination file : this is a directory\n"));
		 	 		return -1;

		}
	 else if(diff(split(destination,1),path_behind(destination))==1 && occurences(path_behind(destination))==0){
	 		write(1, "tsh : destination file : no such file or directory\n",strlen("tsh : destination file : no such file or directory\n"));
	 		return -1;
	 }
	 if(occurences(destination)>0){
		 rm(destination);
		 } 
	 int f = fork();
	 if(f<0){
		 perror("fork error");		 
		 }
	wait(NULL);
	 if(f==0){
		lseek(d,file_end(d),SEEK_SET);
		memset(&hd, '0', 512);
		memset(&hd.linkname, '\0', 100);
		memset(&hd.name, '\0', 100);
		memset(&hd.prefix, '\0', 155);
		memset(&hd.magic, '\0', 6);
		memset(&hd.uname, '\0', 32);
		memset(&hd.gname, '\0', 32);
		memset(&hd.devmajor, '\0', 8);
		memset(&hd.devminor, '\0', 8);
		memset(&hd.prefix, '\0', 155);
		memset(&hd.junk, '\0', 12);
		while(i < strlen(split(destination,2)) && i < 100)
		{
			hd.name[i] = (split(destination,2))[i];
			i++;
		}
		sprintf(hd.mode,"0000775");
		hd.mode[7] = '\0';
		sprintf(hd.mtime, "%o", (int)time(NULL));
		hd.mtime[11] = '\0';
		sprintf(hd.magic, TMAGIC);
		sprintf(hd.version, "  ");
		hd.typeflag = '0';
		sprintf(hd.size, "%011o",MAX_SIZE);
		hd.size[11] = '\0';
		const char *unm = uname();
		strcpy(hd.uname, unm);
		const char *gnm = gname();
		strcpy(hd.gname, gnm);
		set_checksum(&hd);
		lseek(d,file_end(d),SEEK_SET);
		write(d,&hd,512);
		lseek(d,file_end(d),SEEK_SET);
		dup2(d,1);		
		return 0;
	}
		
		return 0;
	
}

/********************redirections >> to a tar file*************/
int redirection2_tar_file(char *destination){
	struct posix_header p;
	struct posix_header hd;
	int d = open(split(destination,1),O_RDWR);
	int i=0,size=0,j=0;
	if(d<0){
		 perror("error while opening destination file ");
		 	 		return -1;

		 }
	 else if(cmp(split(destination,2),"")==0 || destination[strlen(destination)-1]=='/'){
		 write(1, "tsh : destination file : this is a directory\n",strlen("tsh : destination file : this is a directory\n"));
		 	 		return -1;

		}
	 else if(diff(split(destination,1),path_behind(destination))==1 && occurences(path_behind(destination))==0){
	 		write(1, "tsh : destination file : no such file or directory\n",strlen("tsh : destination file : no such file or directory\n"));
	 		return -1;
	 }
	 if(occurences(destination)==0){
		redirection_tar_file(destination);
		 } 
	else{
		lseek(d,0,SEEK_SET);
		while(read(d,&p,512)>0 && p.name[0]!='\0'){
			i+=512;
			if(cmp(split(destination,2),p.name)==0){
					lseek(d,i,SEEK_SET);
					sscanf(p.size, "%o", &size);
					char buffer[size];
					read(d,&buffer,size);
					rm(destination);
					lseek(d,file_end(d),SEEK_SET);
					memset(&hd, '0', 512);
					memset(&hd.linkname, '\0', 100);
					memset(&hd.name, '\0', 100);
					memset(&hd.prefix, '\0', 155);
					memset(&hd.magic, '\0', 6);
					memset(&hd.uname, '\0', 32);
					memset(&hd.gname, '\0', 32);
					memset(&hd.devmajor, '\0', 8);
					memset(&hd.devminor, '\0', 8);
					memset(&hd.prefix, '\0', 155);
					memset(&hd.junk, '\0', 12);
					while(j < strlen(split(destination,2)) && j < 100)
					{
						hd.name[j] = (split(destination,2))[j];
						j++;
					}
					sprintf(hd.mode,"0000775");
					hd.mode[7] = '\0';
					sprintf(hd.mtime, "%o", (int)time(NULL));
					hd.mtime[11] = '\0';
					sprintf(hd.magic, TMAGIC);
					sprintf(hd.version, "  ");
					hd.typeflag = '0';
					sprintf(hd.size, "%011o",MAX_SIZE+size);
					hd.size[11] = '\0';
					const char *unm = uname();
					strcpy(hd.uname, unm);
					const char *gnm = gname();
					strcpy(hd.gname, gnm);
					set_checksum(&hd);
					lseek(d,file_end(d),SEEK_SET);
					write(d,&hd,512);
					lseek(d,file_end(d),SEEK_SET);
					write(d,&buffer,size);
					lseek(d,file_end(d),SEEK_SET);
					dup2(d,1);
					return 0;
			}
		  }
		}
	
	return 0;
	}
	

/*****main************/	
int main(int argc,char **argv){
	int j=0,k=1;
	int n=0;char **args=malloc((argc-1)*sizeof(char *));char *destination;
	for(int i=0;i<argc;i++){
		if(cmp(argv[i],"dd")==0 || cmp(argv[i],"d")==0 ){
			   n+=i;
		}
	}
	printf("n is %d\n",n);	
	if(n!=0){
		
		char *tmp;
			 while(k<argc){ 
				 if(k!=n && k!=(n+1) && j<(argc-1)){
					 tmp=strdup(argv[k]);
					 args[j]=tmp;
					 printf("%s\n",args[j]);
					 j++;
					 k++;
				}
				else{
					k++;
					}
			 }
			 //printf("j %d\n",j);
			
			 destination=strdup(argv[n+1]);
			 printf("file %s\n",destination);
		 }
	redirection_sn(destination);	
	execvp(args[0],args);
return 0;

	}
