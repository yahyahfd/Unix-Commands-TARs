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
#include <pwd.h>
#include <time.h>
#include "rm.h"
#include <grp.h>

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
/*****************************************************************/
int occurences(char *filepath){
	struct posix_header p;
	static int k;
	int fd=open(split(filepath,1),O_RDWR);
	if(fd==-1){
		perror("error while opening the file");
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
	/*****************************************************/
const char *getUserName()
{
	uid_t uid = geteuid();
	struct passwd *pw = getpwuid(uid);
	if (pw)
	{
		return pw->pw_name;
	}

	return "";
}

const char *getGroupName()
{
	gid_t gid = getegid();
	struct group *pw = getgrgid(gid);
	if (pw)
	{
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
	
/***************************************/
int file_position (char *filepath){
	int lus,i;
	struct posix_header p;
	int fd =open(split(filepath,1),O_RDONLY);
	if(fd<0){
		perror("no such file or directory");
		return -1;
		}
	if(occurences(filepath)==0){
		return -1;
		}
	while(lus=read(fd,&p,512)>0 && p.name[0]!='\0'){
		if(cmp(split(filepath,2),p.name)==0){
			return i;
			}
		else{
			i+=512;
			}
		
		}
		return i;
	}
int file_end(int d){
	int end=0;
	struct posix_header p;
	lseek(d,0,SEEK_SET);
	while(read(d,&p,512)>0 && p.name[0]!='\0' ){
		end+=512;
		
		}
	return end;
	}
/***************copy a normal file to a tar one *******/
int copy_normal_to_tar(char *src,char *dest){
		struct stat st;
		struct stat *buf = &st;
		struct posix_header p;
		struct posix_header hd;
		struct group *grp;
		struct passwd *pwd;
		int i=0,j=0,k=0,size,lus,end=0;
		int s=open(src,O_RDONLY);
		int d=open(split(dest,1),O_RDWR);
		if(s<0 ){
			write(1,"cp : source file : no such file or directory\n",strlen("cp : source file : no such file or directory\n"));
			return -1;
			}
				
		if(stat(src,buf)<0){
			perror("stat failed");
			return -1;
			}
		char buffer[buf->st_size];
		char tmp[512];
		if(read(s,buffer,buf->st_size)<0){
			perror("error while reading from source file");
			return -1;
			}
		if(occurences(path_behind(dest))==0){
			write(1,"cp : cannot create file : not a directory \n",strlen("cp : cannot create file : not a directory \n"));
			return -1;
		}
		lseek(d,0,SEEK_SET);
				
	   if(occurences(dest)>0){
			rm(dest);
			copy_normal_to_tar(src,dest);
			return 0;
				}
		
/*****************si aucun file n'existe du nom du fichier source***************************************************************/
		else{
				
				lseek(d,file_end(d)-512,SEEK_SET);	
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
				while (i < strlen(split(dest,2)) && i < 100)
				{
					hd.name[i] = (split(dest,2))[i];
					i++;
				}printf("%s\n",hd.name);
				sprintf(hd.mode, "%o",st.st_mode);
				hd.mode[7] = '\0';
				sprintf(hd.size, "%lo", st.st_size);
				hd.size[11] = '\0';
				sprintf(hd.mtime, "%lo", st.st_mtime);
				hd.mtime[11] = '\0';
				switch (st.st_mode & S_IFMT)
				{
				case S_IFBLK:
					hd.typeflag = '4';
					break;
				case S_IFCHR:
					hd.typeflag = '3';
					break;
				case S_IFDIR:
					hd.typeflag = '5';
					break;
				case S_IFIFO:
					hd.typeflag = '6';
					break;
				case S_IFLNK:
					hd.typeflag = '2';
					break;
				case S_IFREG:
					hd.typeflag = '0';
					break;
				case S_IFSOCK:
					hd.typeflag = 'g';
					break;
				default:
					hd.typeflag = '7';
					break;
				}
				sprintf(hd.magic, TMAGIC);
				sprintf(hd.version, "  ");
				const char *unm = getUserName();
				strcpy(hd.uname, unm);
				const char *gnm = getGroupName();
				strcpy(hd.gname, gnm);
				set_checksum(&hd);
				write(d, &hd, sizeof(hd));
				write(d , &buffer, buf->st_size);
				memset(&p,'\0',512);
				write(d,&p,512);
				write(d,&p,512);
				set_checksum(&hd);
				return 0;
		}
	close(s);
	close(d);
	return 0;
}
/*******************copier un fichier tar dans un fichier normale***********/
int copy_tar_to_normal(char *src,char *dest){
	
	struct stat st;
	struct stat *buf = &st;
	struct posix_header p;
	int size=0,lus,i=0;
	
	int s=open(split(src,1),O_RDONLY);
	int d = open(dest,O_RDWR|O_CREAT|O_TRUNC);
	chmod (dest, 0700);
	if(s<0){
		perror("error while opening source file");
		return -1;
		}
	if(occurences(src)<0){
		write(1,"cp : src file : no such file or directory\n",strlen("cp : src file : no such file or directory\n"));
		return -1;
		}
	
	while(lus=read(s,&p,512)>0 && p.name[0]!='\0'){
		i+=512;
		if(cmp(split(src,2),p.name)==0){
			sscanf(p.size, "%o", &size);
			char *buffer[size];
			lseek(s,i,SEEK_SET);
			if(read(s,&buffer,size)<0){
				perror("error while reading ");
				return -1;
			}
			lseek(d,0,SEEK_SET);
			write(d,&buffer,size);
			}
	}
	close(s);
	close(d);
	return 0;
	
	}
	/**************copier un fichier tar dans un autre fichier tar****************/
int copy_tar_to_tar(char *src,char *dest){
	struct posix_header p1;	
	struct posix_header p2;	
	struct posix_header hd;	
	int i=0,j=0,size,lus,k=0;
	
	int s= open(split(src,1),O_RDONLY);
	int d= open(split(dest,1),O_RDWR);
	
	if(s<0){
		perror("error while opening source file");
		return -1;
		}
	if(d<0){
		perror("error while opening destination file");
		return -1;
		}
	if(occurences(src)==0){
		 write(1,"cp : source file : no such file or directory\n",strlen("cp : source file : no such file or directory\n"));
		return -1;		
	}
	if(cmp(split(dest,2),file_name(dest))!=0 && occurences(path_behind(dest))==0){
		 write(1,"cp : file path : no such file or directory\n",strlen("cp : file path : no such file or directory\n"));
		return -1;		
	}
	
	if(occurences(dest)>0){
		rm(dest);
	}	
		lseek(d,0,SEEK_SET);
		while(lus=read(s,&p1,512)>0 && p1.name[0]!='\0'){
			i+=512;
			if(cmp(split(src,2),p1.name)==0){
				sscanf(p1.size, "%o", &size);
				memset(&p2, '0', 512);
				memset(&p2.linkname, '\0', 100);
				memset(&p2.name, '\0', 100);
				memset(&p2.prefix, '\0', 155);
				memset(&p2.magic, '\0', 6);
				memset(&p2.uname, '\0', 32);
				memset(&p2.gname, '\0', 32);
				memset(&p2.devmajor, '\0', 8);
				memset(&p2.devminor, '\0', 8);
				memset(&p2.prefix, '\0', 155);
				memset(&p2.junk, '\0', 12);
				while (k < strlen(split(dest,2)) && k < 100)
				{
					p2.name[k] = (split(dest,2))[k];
					k++;
				}
				sprintf(p2.mode, "0000775");
				p2.mode[7] = '\0';
				sprintf(p2.size,"%011o",size);
				p2.size[11] = '\0';
				sprintf(p2.mtime, "%011o", (int)time(NULL));
				p2.mtime[11] = '\0';
				p2.mtime[11] = '\0';
				p2.typeflag = '0';
				sprintf(p2.magic, TMAGIC);
				sprintf(p2.version, "  ");
				const char *unm = getUserName();
				strcpy(p2.uname, unm);
				const char *gnm = getGroupName();
				strcpy(p2.gname, gnm);
				set_checksum(&p2);
				lseek(d,file_end(d),SEEK_SET);
				write(d,&p2,512);
				lseek(s,i,SEEK_SET);
				char buffer[size];
				if(read(s,&buffer,size)<0){
					perror("error while reading ");
					return -1;
				}
				lseek(d,file_end(d)+512,SEEK_SET);
				write(d,&buffer,size);					
				memset(&p1,'\0',512);
				write(d,&p1,512);
				write(d,&p1,512);
				set_checksum(&p2);
			return 0;	
			}	
		}
		return -1;
	}
/***********copier un fichier normal dans un dossier tar**************************************/
int cp_fn_tardir(char *src,char *dest){
	
	if(occurences(dest)==0){
		write(1,"cp : destination file : no such file or directory\n",strlen("cp : destination file : no such file or directory\n"));
		return -1;
		}
	char *name=strdup("");
	strcat(name,dest);
	strcat(name,file_name(src));
	copy_normal_to_tar(src,name);
	
	return 0;
	}

/***************copier un fichier tar dans un dossier normal****/
int cp_ftar_dn(char *src ,char *dest){
	struct stat st;
	struct stat *buf = &st;
	struct posix_header p;
	int size=0,lus,i=0;
	int s = open(split(src,1),O_RDONLY);
	if(s<0){
		perror("error while opening source file");
		return -1;
		}
	DIR *dirp = opendir(dest);
	if(dirp==NULL){
		write(1,"cp : destination directory : no such file or directory\n",strlen("cp : destination directory : no such file or directory\n"));
		return -1;
		}
	char *name=strdup("");
	strcat(name,dest);
	strcat(name,file_name(src));
	int d = open(name,O_RDWR|O_CREAT|O_TRUNC);
	chmod (name, 0700);
	
	if(occurences(src)<0){
		write(1,"cp : src file : no such file or directory\n",strlen("cp : src file : no such file or directory\n"));
		return -1;
		}
	while(lus=read(s,&p,512)>0 && p.name[0]!='\0'){
		i+=512;
		if(cmp(split(src,2),p.name)==0){
			sscanf(p.size, "%o", &size);
			char *buffer[size];
			lseek(s,i,SEEK_SET);
			if(read(s,&buffer,size)<0){
				perror("error while reading ");
				return -1;
			}
			lseek(d,0,SEEK_SET);
			write(d,&buffer,size);
			}
		}
	free(name);
	closedir(dirp);
	close(s);
	close(d);
	return 0;
	}
/********************copier un fichier tar dans un dossier tar*****/
int cp_ftar_tardir(char *src ,char *dest){
	struct posix_header p1;
	struct posix_header p2;
	struct posix_header tmp;
	int i=0,size,k=0;
	int s = open(split(src,1),O_RDONLY);
	int d = open(split(dest,1),O_RDWR);
	
	if(s==-1 || d==-1){
		write(1,"cp : cannot open directory \n",strlen("cp : cannot open directory \n"));
		return -1;
		}
		
	if(occurences(src)==0){
		write(1,"cp : source directory : no such file or directory\n",strlen("cp : source directory : no such file or directory\n"));
		return -1;
		}
		
	if(cmp(split(dest,2),"")!=0 && occurences(dest)==0){
		write(1,"cp : destination directory : no such file or directory\n",strlen("cp : destination directory : no such file or directory\n"));
		return -1;
		}
		
	if(src[strlen(src)-1]=='/'){
		write(1,"cp : connot copy file : this is a directory\n",strlen("cp : connot copy file : this is a directory\n"));
		return -1;
		}
			
	char *name=strdup("");
	strcat(name,dest);
	if( dest[strlen(dest)-1]!='/'){
		strcat(name,"/");
		}
	strcat(name,file_name(src));	
	if(cmp(split(src,2),"")!=0 && occurences(path_behind(src))==0){
		write(1,"cp : destination directory : no such file or directory\n",strlen("cp : destination directory : no such file or directory\n"));
		return -1;
		}
		
	else if(occurences(name)>0){
		rm(name);
	}
	
	while(read(s,&p2,512)>0 && p2.name[0]!='\0'){
		i+=512;
		if(cmp(split(src,2),p2.name)==0){
			sscanf(p2.size, "%o", &size);
			char *buffer[size];			
			lseek(s,i-512,SEEK_SET);
			read(s,&p2,512);			
				memset(&p2, '0', 512);
				memset(&p2.linkname, '\0', 100);
				memset(&p2.name, '\0', 100);
				memset(&p2.prefix, '\0', 155);
				memset(&p2.magic, '\0', 6);
				memset(&p2.uname, '\0', 32);
				memset(&p2.gname, '\0', 32);
				memset(&p2.devmajor, '\0', 8);
				memset(&p2.devminor, '\0', 8);
				memset(&p2.prefix, '\0', 155);
				memset(&p2.junk, '\0', 12);
				char *n=strdup("");
				strcat(n,split(dest,1));
				strcat(n,"/");
				strcat(n,path_behind(split(src,2)));
				if(occurences(n)>0){
					while (k < strlen(split(src,2)) && k < 100)
					{
						p2.name[k] = (split(src,2))[k];
						k++;
					}
				}
			else {
				while (k < strlen(file_name(src)) && k < 100)
					{
						p2.name[k] = (file_name(src))[k];
						k++;
					}
				
				}
				sprintf(p2.mode, "0000775");
				p2.mode[7] = '\0';
				sprintf(p2.size,"%011o",size);
				p2.size[11] = '\0';
				sprintf(p2.mtime, "%011o", (int)time(NULL));
				p2.mtime[11] = '\0';
				p2.mtime[11] = '\0';
				p2.typeflag = '0';
				sprintf(p2.magic, TMAGIC);
				sprintf(p2.version, "  ");
				const char *unm = getUserName();
				strcpy(p2.uname, unm);
				const char *gnm = getGroupName();
				strcpy(p2.gname, gnm);
				set_checksum(&p2);
				printf(" p2 name %s\n",p2.name);
				lseek(d,file_end(d),SEEK_SET);
				write(d,&p2,512);
				lseek(s,i,SEEK_SET);
				if(read(s,&buffer,size)<0){
					perror("error while reading ");
					return -1;
				}
				lseek(d,file_end(d),SEEK_SET);
				write(d,&buffer,size);
				lseek(d,file_end(d),SEEK_SET);
				memset(&tmp,'\0',512);
				write(d,&tmp,512);
				write(d,&tmp,512);
				free(n);
			}
		}
	return 0;
	}
/**********copy a tar directory to another **********/
int copy_tars(char *src ,char *dest){
	struct posix_header p1;
	struct posix_header p2;
	int size=0,i=0,k=0;
	int s = open(src,O_RDONLY);
	int d = open(split(dest,1),O_RDWR);
	
	if(s==-1 || d==-1){
		perror("error while opening the file ");
		return -1;
		}
	lseek(s,0,SEEK_SET);
	while(read(s,&p1,512)>0 &&  p1.name[0]!='\0'){
		lseek(d,0,SEEK_SET);
		if(p1.typeflag=='5'){
			char *namedir=strdup("");
			strcat(namedir,split(dest,1));
			strcat(namedir,"/");
			strcat(namedir,p1.name);
			if(occurences(namedir)==0){
				lseek(d,file_end(d),SEEK_SET);
				write(d,&p1,512);
				memset(&p1,'\0',512);
				write(d,&p1,512);
				set_checksum(&p1);
				free(namedir);
				}
			}
			
		else{
			
			sscanf(p1.size, "%o", &size);
			char *named=strdup("");
			strcat(named,split(dest,1));
			strcat(named,"/");
			strcat(named,p1.name);
			if(occurences(named)>0){
				rm(named);
				}
			char *names=strdup("");
			strcat(names,split(src,1));
			strcat(names,"/");
			strcat(names,p1.name);
			cp_ftar_tardir(names,path_behind(named));
			i=0;
			while(i<((size + 512 - 1) / 512)){
				read(s,&p1,512);
				i++;
				}
			free(names);
			free(named);
			
			}
		}
	return 0;
	}

int cp_ndir_tardir(char *src ,char *dest){
	 
	
	
	
	}



int main(int argc,char **argv){
	copy_tars(argv[1],argv[2]);
	//copy_tar_to_normal(argv[1],argv[2]);
	//copy_tar_to_tar(argv[1],argv[2]);
	//cp_ftar_dn(argv[1],argv[2]);
	//cp_ftar_tardir(argv[1],argv[2]);
	//printf("%s\n",path_behind(argv[1]));
	//int f=open(argv[1],O_RDONLY);
	
	return 0;
	}	
/*fichiers :

normal ===> tar done
tar   ===> normal done
tar  ===> tar done

fichiers et dossiers :

tar ==> dossier normal done
tar ===> dossier tar done
normal ===> dossier tar done

dossiers : 

dossier tar ==> dossier tar done
dossier normal ==> dosssier tar ***********
dossier tar ===> dossier normal **********
*/
