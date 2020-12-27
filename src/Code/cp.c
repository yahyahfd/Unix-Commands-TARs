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


	/**************************returns the number of occurences of a file***************************************/
	int occurences1(char *filepath){
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
		char *name;
		int f=fork();
		if(f==-1){
			perror("fork error");
			exit(-1);
		}
		wait(NULL);
		if(f==0){
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
				
			name=strtok(tmp2,"/");
			for(int i=0;i<nb-1;i++){
				name=strdup(strtok(NULL,"/"));

				}
				return name;
		}
		free(name);
	}
		
	/***************copy a normal file to a tar one *******/
	int cp_nf_tarf(char *src,char *dest){
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
				perror("error while reading from source file\n");
				return -1;
				}

			if(diff(split(dest,1),path_behind(dest))!=1 && occurences1(path_behind(dest))==0){
				write(1,"cp : cannot create file : not a directory \n",strlen("cp : cannot create file : not a directory \n"));
				return -1;
			}
			lseek(d,0,SEEK_SET);
					
		   if(occurences1(dest)>0){
				rm(dest);
				}
			
	/*****************we create the file***************************************************************/
					
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
					while(i < strlen(split(dest,2)) && i < 100)
					{
						hd.name[i] = (split(dest,2))[i];
						i++;
					}
					sprintf(hd.mode, "%o",st.st_mode);
					hd.mode[7] = '\0';
					sprintf(hd.size, "%lo", st.st_size);
					hd.size[11] = '\0';
					sprintf(hd.mtime, "%lo", st.st_mtime);
					hd.mtime[11] = '\0';
					sprintf(hd.magic, TMAGIC);
					sprintf(hd.version, "  ");
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
					default:
						hd.typeflag = '7';
						break;
					}
					
					const char *unm = uname();
					strcpy(hd.uname, unm);
					const char *gnm = gname();
					strcpy(hd.gname, gnm);
					set_checksum(&hd);
					lseek(d,file_end(d),SEEK_SET);
					write(d, &hd, sizeof(hd));
					lseek(d,file_end(d),SEEK_SET);
					write(d , &buffer, buf->st_size);
					memset(&p,'\0',512);
					write(d,&p,512);
					write(d,&p,512);
					set_checksum(&hd);
					return 0;
			
		close(s);
		close(d);
		return 0;
	}
	/*******************copy a tar file in a normal file***********/
	int cp_tarf_nf(char *src,char *dest){
		
		struct stat st;
		struct stat *buf = &st;
		struct posix_header p;
		int size=0,lus,i=0;
		
		int s=open(split(src,1),O_RDONLY);
		int d = open(dest,O_RDWR|O_CREAT|O_TRUNC);
		chmod (dest, 0700);
		if(s<0){
			perror("error while opening source file\n");
			return -1;
			}
		if(occurences1(src)<0){
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
		/**************copy a tar file in a tar file****************/
	int cp_ftar_ftar(char *src,char *dest){
		struct posix_header p1;	
		struct posix_header p2;	
		struct posix_header hd;	
		int i=0,j=0,size,lus,k=0;
		
		int s= open(split(src,1),O_RDONLY);
		int d= open(split(dest,1),O_RDWR);
		
		if(s<0){
			perror("error while opening source file\n");
			return -1;
			}
		if(d<0){
			perror("error while opening destination file\n");
			return -1;
			}
		if(occurences1(src)==0){
			 write(1,"cp : source file : no such file or directory\n",strlen("cp : source file : no such file or directory\n"));
			return -1;		
		}
		if(cmp(split(dest,2),file_name(dest))!=0 && occurences1(path_behind(dest))==0){
			 write(1,"cp : file path : no such file or directory\n",strlen("cp : file path : no such file or directory\n"));
			return -1;		
		}
		
		if(occurences1(dest)>0){
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
					const char *unm = uname();
					strcpy(p2.uname, unm);
					const char *gnm = gname();
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
	/***********copy a normal file inside a tar directory**************************************/
	int cp_fn_tardir(char *src,char *dest){
		
		if(cmp(split(dest,2),"")==0 || cmp(split(dest,2),"echec")==0){
			char *newf = strdup("");
			strcat(newf,split(dest,1));
			strcat(newf,"/");
			strcat(newf,file_name(src));
			cp_nf_tarf(src,newf);
			free(newf);
		}
		else{
			char *name=strdup("");
			strcat(name,dest);
			strcat(name,file_name(src));
			cp_nf_tarf(src,name);
			free(name);
		}
		return 0;
	}

	/***************copy a tar file in a normal directory****/
	int cp_ftar_dn(char *src ,char *dest){
		
		struct stat st;
		struct stat *buf = &st;
		struct posix_header p;
		int size=0,lus,i=0;
		int s = open(split(src,1),O_RDONLY);
		if(s<0){
			perror("error while opening source file\n");
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
		
		if(occurences1(src)<0){
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
					perror("error while reading \n");
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
	/********************copy a tar file inside a tar directory*****/
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
			
		if(occurences1(src)==0){
			write(1,"cp : source directory : no such file or directory\n",strlen("cp : source directory : no such file or directory\n"));
			return -1;
			}
			
		if(cmp(split(dest,2),"")!=0 && occurences1(dest)==0){
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
		if(cmp(split(src,2),"")!=0 && diff(split(src,1),path_behind(src))!=1 && occurences1(path_behind(src))==0){
			write(1,"cp : source directory : no such file or directory\n",strlen("cp : source directory : no such file or directory\n"));
			return -1;
			}
			
		if(occurences1(name)>0){
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
					if(diff(split(dest,1),path_behind(dest))!=1){	
						strcat(n,dest);
						strcat(n,file_name(src));
						if(occurences1(path_behind(n))>0){
							while (k < strlen(split(n,2)) && k < 100)
							{
								p2.name[k] = (split(n,2))[k];
								k++;
							}
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
					const char *unm = uname();
					strcpy(p2.uname, unm);
					const char *gnm = gname();
					strcpy(p2.gname, gnm);
					set_checksum(&p2);
					lseek(d,file_end(d),SEEK_SET);
					write(d,&p2,512);
					lseek(s,i,SEEK_SET);
					if(read(s,&buffer,size)<0){
						perror("error while reading \n");
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
		free(name);
		close(s);
		close(d);
		return 0;
		}
	/**********copy a tar directory in another tar directory **********/
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
				if(occurences1(namedir)==0){
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
				if(occurences1(named)>0){
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
		close(s);
		close(d);
		return 0;
		}
	/********************copy a normal directory inside a tar directory******/
	int cp_ndir_tardir(char *src ,char *dest){
		
		struct stat st;
		struct stat *buf = &st;
		struct stat st2;
		struct stat *buf2 = &st2;
		struct dirent *dt;
		struct posix_header p;
		struct posix_header p2;
		int i=0,j=0,b=-1,size=0;
		int d=open(split(dest,1),O_RDWR);
		
		if(dest<0){
			write(1,"cp -r : error while opening destination file \n",strlen("cp -r : error while opening destination file\n"));
			return -1;
			}
		
		DIR *dirp = opendir(src);
		if(dirp==NULL){
			write(1,"cp -r : source directory : no such file or directory\n",strlen("cp -r : source directory : no such file or directory\n"));
			return -1;
			}
		while((dt=readdir(dirp))!=NULL){
			
			if(b==-1){
					char *destname=strdup("");
					strcat(destname,dest);
					if(destname[strlen(destname)-1]!='/')
							strcat(destname,"/");			
					strcat(destname,src);
					if(src[strlen(src)-1]!='/')
						strcat(destname,"/");
					if(occurences1(destname)>0){
						b=1;
						}
					else{
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
					while (j < strlen(split(destname,2)) && j < 100)
					{
						p2.name[j] = (split(destname,2))[j];
						j++;
					}
					sprintf(p2.mode, "%o",st2.st_mode);
					p2.mode[7] = '\0';
					sprintf(p2.size,"%011o",size);
					p2.size[11] = '\0';
					sprintf(p2.mtime, "%lo", st2.st_mtime);
					p2.mtime[11] = '\0';
					p2.typeflag = '5';
					sprintf(p2.magic, TMAGIC);
					sprintf(p2.version, "  ");
					const char *u = uname();
					strcpy(p2.uname, u);
					const char *g = gname();
					strcpy(p.gname, g);
					set_checksum(&p2);
					lseek(d,file_end(d),SEEK_SET);
					write(d,&p2,512);
					free(destname);
					b=1;
					}
				}
				if(cmp(dt->d_name,".")!=0 && cmp(dt->d_name,"..")!=0){
					char *name=strdup("");
					strcat(name,src);
					if(src[strlen(src)-1]!='/')
							strcat(name,"/");
					strcat(name,dt->d_name);
					if(stat(name,buf)<0){
							perror("stat error");
							return -1;
							}
				
					if((buf->st_mode & S_IFMT)== S_IFREG){
						char *destname=strdup("");
						strcat(destname,dest);
						if(destname[strlen(destname)-1]!='/')
								strcat(destname,"/");			
						strcat(destname,src);
						if(src[strlen(src)-1]!='/')
							strcat(destname,"/");
						strcat(destname,dt->d_name);
						cp_nf_tarf(name,destname);
					}
					else if((buf->st_mode & S_IFMT)== S_IFDIR){
						
							strcat(name,"/");
							memset(&p, '0', 512);
							memset(&p.linkname, '\0', 100);
							memset(&p.name, '\0', 100);
							memset(&p.prefix, '\0', 155);
							memset(&p.magic, '\0', 6);
							memset(&p.uname, '\0', 32);
							memset(&p.gname, '\0', 32);
							memset(&p.devmajor, '\0', 8);
							memset(&p.devminor, '\0', 8);
							memset(&p.prefix, '\0', 155);
							memset(&p.junk, '\0', 12);
							while (i < strlen(name) && i < 100)
							{
								p.name[i] = name[i];
								i++;
							}
							sprintf(p.mode, "%o",st.st_mode);
							p.mode[7] = '\0';
							sprintf(p.size, "%lo", st.st_size);
							p.size[11] = '\0';
							sprintf(p.mtime, "%lo", st.st_mtime);
							p.mtime[11] = '\0';
							p.typeflag = '5';
							sprintf(p.magic, TMAGIC);
							sprintf(p.version, "  ");
							const char *unm = uname();
							strcpy(p.uname, unm);
							const char *gnm = gname();
							strcpy(p.gname, gnm);
							set_checksum(&p);
							lseek(d,file_end(d),SEEK_SET);
							write(d, &p,512);
							lseek(d,file_end(d),SEEK_SET);
							memset(&p,'\0',512);
							write(d,&p,512);
							write(d,&p,512);
							set_checksum(&p);
							name[strlen(name)-1]='\0';
							cp_ndir_tardir(name,dest);
						}
						free(name);
				}
			else{
				}
		}
		closedir(dirp);
		close(d);
		return 0;
	}
	/*****************copy a tar directory inside a normal directory*****/
	int cp_tardir_ndir(char *src ,char *dest){
		 struct posix_header p;
		 int size,i,lus;
		 struct dirent *dt;
		 DIR *dirp = opendir(dest);
		 int s=open(split(src,1),O_RDONLY);
		 dirp = opendir(dest);
		 
		 if(s<0){
			 write(1,"cp -r : source directory : no such file or directory\n",strlen("cp -r : source directory : no such file or directory\n"));
			return -1;
			}
		 if(dirp==NULL && cmp(path_behind(dest),"")==0){
			if( mkdir(dest,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)<0){
				write(1,"cp -r : destination directory : can not create directory\n",strlen("cp -r : destination directory : can not create directory\n"));
				return -1;
				}
		 }
		 else if(dirp==NULL && opendir(path_behind(dest))==NULL){
			 write(1,"cp -r : destination directory : can not create directory\n",strlen("cp -r : destination directory : can not create directory\\n"));
			return -1;
			}
		lseek(s,0,SEEK_SET);
		while(read(s,&p,512)>0 &&  p.name[0]!='\0'){
			if(cmp(split(src,2),p.name)==0){
				if(p.typeflag!='5'){
					 write(1,"cp -r : this is a file not a directory\n",strlen("cp -r : this is a file not a directory\n"));
					return -1;
					}
				char *name=strdup("");
				strcat(name,dest);
				if(dest[strlen(dest)-1]!='/')
					strcat(name,"/");
				strcat(name,file_name(p.name));
				strcat(name,"/");
				mkdir(name,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
				free(name);
				}
			else{
				
				if(cmp(p.name,split(src,2))!=0 && diff(split(src,2),p.name)==-1){
					if(p.typeflag!='5'){
							char *n=strdup("");
							strcat(n,split(src,1));
							strcat(n,"/");
							strcat(n,p.name);
							char *d=strdup("");
							if(cmp(path_behind(dest),"")!=0)
								strcat(d,path_behind(dest));
							else
								strcat(d,dest);
							if(dest[strlen(dest)-1]!='/')
								strcat(d,"/");
							strcat(d,p.name);
							char *source = strdup(n);
							char *desti = strdup(d);
							free(n);
							free(d);
							cp_tarf_nf(source,desti);
							}
						
					else if(p.typeflag=='5'){
						char *n=strdup("");
						strcat(n,split(src,1));
						strcat(n,"/");
						strcat(n,p.name);
						char *d=strdup("");
						strcat(d,dest);
						if(dest[strlen(dest)-1]!='/')
							strcat(d,"/");
						strcat(d,path_behind(p.name));
						char *source = strdup(n);
						char *desti = strdup(d);
						free(n);
						free(d);
						cp_tardir_ndir(source,desti);
							}
						}
				}
				sscanf(p.size, "%o", &size);
				i=0;
					while(i<((size + 512 - 1) / 512)){
						read(s,&p,512);
						i++;
					}		
			}
		close(s);
		closedir(dirp);
		 return 0;
	}


	
int main(int argc,char **argv){
	int j=1,option=-1,n=0;
	
	if(argc<=1){
		 perror("cp : missing operand ");
		 exit(-1);
		}
	
	
	//verifie si l'un des arguments est une option
	while(j<argc && option!=0){
		if(argv[j][0]=='-' ){
			option=1;
			if(argv[j][1]=='r'){
				option=0;
				n+=j;
			}
					
					}
			j++;
		}
		
	if(argc>3 && option==-1 && cmp(split(argv[1],1),"echec")!=0 &&  cmp(split(argv[2],1),"echec")!=0 ){
		 perror("cp : too many arguments");
		 exit(-1);
		}	
		
	if(option==1){
		execvp("/bin/cp",argv);
		}		

		if(option==-1){
			if(argc==3){
				if(cmp(split(argv[1],1),"echec")==0 && cmp(split(argv[2],1),"echec")!=0 && cmp(split(argv[2],2),"")!=0 && opendir(argv[1])==NULL){
					cp_nf_tarf(argv[1],argv[2]);
					} 
				else if(cmp(split(argv[1],1),"echec")!=0 && cmp(split(argv[1],2),"")!=0 && argv[1][strlen(argv[1])-1]!='/' && cmp(split(argv[2],1),"echec")==0 && opendir(argv[2])==NULL ){
					cp_tarf_nf(argv[1],argv[2]);
					}
				else if(cmp(split(argv[1],1),"echec")!=0 && cmp(split(argv[1],2),"")!=0 && argv[1][strlen(argv[1])-1]!='/' && cmp(split(argv[2],1),"echec")!=0 && cmp(split(argv[2],2),"")!=0 && argv[2][strlen(argv[2])-1]!='/' ){
					cp_ftar_ftar(argv[1],argv[2]);
					}
				else if(cmp(split(argv[1],1),"echec")!=0 && cmp(split(argv[1],2),"")!=0 && argv[1][strlen(argv[1])-1]!='/' && cmp(split(argv[2],1),"echec")==0 && (opendir(argv[2])!=NULL || argv[2][strlen(argv[2])-1]=='/')){
					cp_ftar_dn(argv[1],argv[2]);
					}
				else if(cmp(split(argv[1],1),"echec")!=0 && cmp(split(argv[1],2),"")!=0 && argv[1][strlen(argv[1])-1]!='/' && cmp(split(argv[2],1),"echec")!=0 && (argv[2][strlen(argv[2])-1]=='/' || cmp(split(argv[2],2),"")==0)){
					cp_ftar_tardir(argv[1],argv[2]);
					}
				else if(cmp(split(argv[1],1),"echec")==0 && opendir(argv[1])==NULL && cmp(split(argv[2],1),"echec")!=0 && (argv[2][strlen(argv[2])-1]=='/' || cmp(split(argv[2],2),"")==0)){
					cp_fn_tardir(argv[1],argv[2]);
					}
				else{
					write(1,"cp : can not copy directories without -r option\n",strlen("cp : can not copy directories without -r option\n"));
					return -1;
					}
				}
			}
		else if(option==0){
			if(argc==4){
				if(cmp(argv[1],"-r")==0){
					if(cmp(split(argv[2],1),"echec")!=0 && (argv[2][strlen(argv[2])-1]=='/' || cmp(split(argv[2],2),"")==0) && cmp(split(argv[3],1),"echec")!=0 && (argv[3][strlen(argv[3])-1]=='/' || cmp(split(argv[3],2),"")==0)){	
							copy_tars(argv[2],argv[3]);	
					}
					else if(cmp(split(argv[2],1),"echec")==0 && opendir(argv[2])!=NULL && cmp(split(argv[3],1),"echec")!=0 && (argv[3][strlen(argv[3])-1]=='/' || cmp(split(argv[3],2),"")==0)){	
							cp_ndir_tardir(argv[2],argv[3]);	
						}
					else if( cmp(split(argv[2],1),"echec")!=0 && (argv[2][strlen(argv[2])-1]=='/' || cmp(split(argv[2],2),"")==0) && cmp(split(argv[3],1),"echec")==0 && (opendir(argv[3])!=NULL || argv[3][strlen(argv[3])-1]=='/')){	
							printf("hahahah");cp_tardir_ndir(argv[2],argv[3]);	
						}
					}
						
				else if(cmp(argv[2],"-r")==0){
					if(cmp(split(argv[1],1),"echec")!=0 && (argv[1][strlen(argv[1])-1]=='/' || cmp(split(argv[1],2),"")==0) && cmp(split(argv[3],1),"echec")!=0 && (argv[3][strlen(argv[3])-1]=='/' || cmp(split(argv[3],2),"")==0)){	
							copy_tars(argv[1],argv[3]);	
					}
					else if(cmp(split(argv[1],1),"echec")==0 && opendir(argv[1])!=NULL && cmp(split(argv[3],1),"echec")!=0 && (argv[3][strlen(argv[3])-1]=='/' || cmp(split(argv[3],2),"")==0)){	
							cp_ndir_tardir(argv[1],argv[3]);	
						}
					else if( cmp(split(argv[1],1),"echec")!=0 && (argv[1][strlen(argv[1])-1]=='/' || cmp(split(argv[1],2),"")==0) && cmp(split(argv[3],1),"echec")==0 && (opendir(argv[3])!=NULL || argv[3][strlen(argv[3])-1]=='/')){	
							cp_tardir_ndir(argv[1],argv[3]);	
						}
					}	
				else if(cmp(argv[3],"-r")==0){
					if(cmp(split(argv[1],1),"echec")!=0 && (argv[1][strlen(argv[1])-1]=='/' || cmp(split(argv[1],2),"")==0) && cmp(split(argv[2],1),"echec")!=0 && (argv[2][strlen(argv[2])-1]=='/' || cmp(split(argv[2],2),"")==0)){	
							copy_tars(argv[1],argv[2]);	
					}
					else if(cmp(split(argv[1],1),"echec")==0 && opendir(argv[1])!=NULL && cmp(split(argv[2],1),"echec")!=0 && (argv[2][strlen(argv[2])-1]=='/' || cmp(split(argv[2],2),"")==0)){	
							cp_ndir_tardir(argv[1],argv[2]);	
						}
					else if( cmp(split(argv[1],1),"echec")!=0 && (argv[1][strlen(argv[1])-1]=='/' || cmp(split(argv[1],2),"")==0) && cmp(split(argv[2],1),"echec")==0 && (opendir(argv[2])!=NULL || argv[2][strlen(argv[2])-1]=='/')){	
							
							cp_tardir_ndir(argv[1],argv[2]);	
						}
					}	
				}
			}
		return 0;
	}

