
/**returns the number of occurences of a files inside a tar***/
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
		return k;

	}
/***remove an empty directory********/
int remove_dir(char *filepath){
	struct stat st;
	struct stat *buf = &st;
	struct posix_header p;
	int lus,i=0,j=0;
	int fd=open(split(filepath,1),O_RDWR);
	if(fd==-1){
	    write(1, "rmdir: error while opening the file\n", strlen("rmdir: error while opening the file\n"));
		exit(-1);
		}
		/*****deleting an emty tar***************/

	if(cmp(split(filepath,1),filepath)==0 )
	{
		read(fd,&p,512);
		if(p.name[0]=='\0'){
			stat(filepath,buf);
			if(unlink(filepath)<0){
				perror("unlink error");
				exit(-1);
				}
			exit(EXIT_SUCCESS);
		}
			else
			{
			 write(1, "rmdir: This directory is not empty\n", strlen("rmdir: This directory is not empty\n"));
			exit(EXIT_FAILURE);
			}
		}
	
	/*******deleting a directory inside a tar***/
	else
	    { 
		
		if(occurences(filepath)==0){								  
							   write(1, "rmdir: No such file or directory\n", strlen("rmdir: No such file or directory\n"));
								exit(EXIT_FAILURE);
							}
		lseek(fd,0,SEEK_SET);
		while(read(fd,&p,512)>0){
			i+=512;
			j+=512;
			 
			if(cmp(split(filepath,2),p.name)==0){
						if(p.typeflag!='5'){
							 write(1, "rmdir: This is not a directory\n", strlen("rmdir: This is not a directory\n"));			
							 exit(EXIT_FAILURE);
							 }
								
						else if(occurences(filepath)>1){								  
							   write(1, "rmdir: This directory is not empty\n", strlen("rmdir: This directory is not empty\n"));
								exit(EXIT_FAILURE);
							}
							  
						else{
							lseek(fd,i,SEEK_SET);
							read(fd,&p,512);
							if(	p.name[0]=='\0'){
								memset(&p,'\0',512);
								lseek(fd,i-512,SEEK_SET);
								write(fd,&p,512);
								set_checksum(&p);
								exit(0);
								}
							else{
								lseek(fd,i,SEEK_SET);
								while((lus=read(fd,&p,512))>0 && p.name[0]!='\0'){
									lseek(fd,i-512,SEEK_SET);
									i+=512;
									write(fd,&p,lus);
									lseek(fd,512,SEEK_CUR);
									j+=512;
									}				
								memset(&p,'\0',512);
								lseek(fd,j-512,SEEK_SET);
								write(fd,&p,512);
								set_checksum(&p);
								exit(0);
							}
					  }
					}
			lseek(fd,i,SEEK_SET);
			
			}
		return 0;
		}
	return -1;
}

/***main function of rmdir****/	
int tsh_rmdir(int argc, char **argv){
	int i=1,option=-1;
	
	if(argc<=1){
		 perror("Rmdir : missing operand ");
		 exit(-1);
		 
		}
	
	//verifie si l'un des arguments est une option
	while(i<argc){
		if(argv[i][0]=='-' ){
				option=0;	
					}
			i++;
		}
	if(option==0){
		execvp("/bin/rmdir",argv);
	}
	
	else{
			
		for(int i=1;i<argc;i++){	
			if(cmp(split(argv[i],1),"echec")==-1 && cmp(split(argv[i],2),"echec")==-1 ){
						remove_dir(argv[i]);
				}
					else{
						//sinon on execute la commande rmdir usuelle
						int pid=fork();
						if(pid<0)
							return (EXIT_FAILURE);
						if(pid==0){
								execlp("rmdir","rmdir",argv[i],NULL);
							}		
							waitpid(pid,NULL,0);
							}
						}	
		}
	return 0;

}
	
