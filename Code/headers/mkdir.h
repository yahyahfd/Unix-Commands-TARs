	/******************************mdkir**************************/
  	int tsh_mkdir(int argc, char **argv){//options are not working
      int opts=-1; //counter for options, equals -1 if there is no option, equals 0 if there is an option
      int tar=-1; //-1 if no tarball, 0 if there is a tarball
      for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){//argv[i] is an option
          opts=0;
        }
        if(cmp(split(argv[i],1),"echec")==-1){//tarball
          tar=0;
        }
      }
      if(opts==0){//at least one option
        if(tar==0){//at least one tarball involved
          perror("Mkdir: mkdir with options does not work on tarballs");
          exit(EXIT_FAILURE);
        }else{//no tarball
          execvp("mkdir",argv);
        }
      }else{
        if(tar==0){//at least one tarball involved
          if(argc==2){//mkdir takes one argument
            //we first check if the dir we want to make already exists
            struct posix_header p;
            int fd=open(split(argv[1],1),O_RDONLY),lus,i=0;
            if(fd==-1){
              write(1,"mkdir : no such file or directory\n",strlen("mkdir : no such file or directory\n"));
              exit(EXIT_FAILURE);
            }
            if(cmp(split(argv[1],2),"")==0){
              write(1,"mkdir : you cannot create a tar with mkdir\n",strlen("mkdir : you cannot create a tar with mkdir\n"));
              return -1;
            }
            do {
              lus=read(fd,&p,512);
            }
            while(cmp(p.name,split(argv[1],2))==-1 && lus>0);
            if(p.typeflag=='5'){//the specified directory already exists so we cannot make it
              write(1,"mkdir : this directory already exists\n",strlen("mkdir : this directory already exists\n"));
              exit(EXIT_FAILURE);
            }else{//there is no directory with this name, so we can still create it
              lseek(fd,0,SEEK_SET);
              while(read(fd,&p,512)>0){
                i+=512;
                time_t now = time(0);
                char timetochar[sizeof(now)];
                sprintf(timetochar,"%ld",now);
                if(p.name[0]=='\0'){//empty block, because no name, no directory/file here -> we create our directory here
                  strcpy(p.name,split(argv[1],2));//name
                  strcpy(p.mode,"644");//permissions
                  p.typeflag=5;//file type, 5 is directory
                  strcpy(p.mtime,timetochar);//mtime is creation time here
                  set_checksum(&p);
                }else{
                  while((lus=read(fd,&p,512))>0 && p.name[0]!='\0'){//while current block is not empty, we read each block until we get an empty block
                    lseek(fd,i-512,SEEK_SET);
                    i+=512;
                    write(fd,&p,lus);
                    lseek(fd,512,SEEK_CUR);
                  }
                }
              }
            }
          }else{
            perror("Mkdir: mkdir only takes one argument");
            exit(EXIT_FAILURE);
          }
        }else{//no tarball
          execvp("mkdir",argv);
        }
      }
      return 0;
    }
