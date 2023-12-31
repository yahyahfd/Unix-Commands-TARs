	
	/**********view contain of file **********/
	int contenu_tar(char *chemin,char *filename){
		struct posix_header p;
		char buffer[512];
		int fd=open(chemin,O_RDONLY),lus,size;
		if(fd==-1){
			write(1,"cat : no such file or directory\n",strlen("cat : no such file or directory\n"));
			exit(EXIT_FAILURE);
		}
		if(cmp(filename,"")==0){
			write(1,"cat : this is a directory\n",strlen("cat : this is a directory\n"));
			return -1;
			}
			do {
				lus=read(fd,&p,512);
			}	
			while(cmp(p.name,filename)==-1 && lus>0);
				
			if(cmp(p.name,filename)==0){
				if(p.typeflag=='5'){
						write(1,"cat : this is a directory\n",strlen("cat : this is a directory\n"));
						return -1;
					}
				sscanf(p.size,"%o",&size);
				for(int i=0;i<((size+512-1)/512);i++){
					lus=read(fd,buffer,512);
					write(1,buffer,lus);
				}	
					close(fd);
				 return 0;
				 
					}
			close(fd);
			write(1,"cat : no such file or directory\n",strlen("cat : no such file or directory\n"));
			return -1;
	}
	
	
	/***************cat main function*****************/
		
	int tsh_cat(int argc, char **argv){
		int i=1,option=-1;
		//verify if one the arguments is an option
		if(argc==1){
			execvp("/bin/cat",argv);
			}
		while(i<argc){
			if(argv[i][0]=='-' ){
					option=0;	
						}
				i++;
			}
		if(option==0){
			execvp("/bin/cat",argv);
		}
		
		else{
				
			for(int i=1;i<argc;i++){	
				//if the file path contains a tarball
				if(cmp(split(argv[i],1),"echec")==-1 )
						{
							contenu_tar(split(argv[i],1),split(argv[i],2));
							}
						else{
							//we execute the usuall cat command
							int pid=fork();
							if(pid<0)
								return (EXIT_FAILURE);
							if(pid==0){
									execlp("cat","cat",argv[i],NULL);
								}		
								waitpid(pid,NULL,0);
								}
						}	
			}
	
		return 0;
	
	}
	
	
