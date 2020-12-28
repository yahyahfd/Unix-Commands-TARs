


/***remove a file ***/
int rm(char *filepath){
	struct posix_header p;
	int i=0,j=0,size,lus;
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
		write(1,"rm :Cannot delete file :  No such file or directory \n",strlen("rm :Cannot delete file :  No such file or directory \n"));
		return 0;
	}
/***remove files and directories recursivly***/
int rmr(char *filepath){
	struct posix_header p;
	int i=0;
	int fd=open(split(filepath,1),O_RDWR);
	if(fd==-1 ){
		write(1,"rm : No such file or directory \n",strlen("rm : 2No such file or directory \n"));
		return -1;
		}
	//we want to delete a tarball
	if(cmp(split(filepath,1),filepath)==0){
		if(unlink(filepath)==-1){
			write(1,"rm : unlik error :Cannot delete file\n",strlen("rm : unlik error :Cannot delete file\n"));
			return -1;
			}
		return 0;
		}
	//we want to delete a directory inside a tarball
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
	write(1,"rm : No such file or directory \n",strlen("rm : No such file or directory \n"));
	return -1;
}

int tsh_rm(int argc,char **argv){
	int j=1,option=-1,n=0;
	
	if(argc<=1){
		 write(1,"rm : missing operand \n",strlen("rm : missing operand \n"));
		 exit(-1);
		}
	
	//verify if one of the arguments is an option
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
		if(execvp("/bin/rm",argv)==-1)
			return -1;
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
							//we execute the usuall rm command
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
