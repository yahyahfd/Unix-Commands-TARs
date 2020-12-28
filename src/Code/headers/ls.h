

int ls_tar(char *chemin, int isl){
  struct posix_header tarstr;
  char buffer[512];
  int fd = open(chemin,O_RDONLY,0),lus,size;
  const char *rights[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-","rwx", NULL };

  if (fd==-1) {
    perror("This file does not exist");
    exit(EXIT_FAILURE);
  }

  do {
    lus=read(fd, &tarstr, 512);
    if(tarstr.name[0]!='\0'){
      int m=0;
      while(m<100 && tarstr.name[m]!='\0'){
        m++;
      }
      if(isl==1){
        if(tarstr.typeflag=='0'){
          write(STDOUT_FILENO, "-",1);
        }else if(tarstr.typeflag=='5'){
          write(STDOUT_FILENO, "d",1);
        }
        int v= atoi(tarstr.mode);
        write(STDOUT_FILENO, rights[v/100],3);
        write(STDOUT_FILENO, rights[(v%100)/10],3);
        write(STDOUT_FILENO, rights[(v%100)%10],3);
        write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, tarstr.uname, strlen(tarstr.uname));
        write(STDOUT_FILENO, "/", 1);
        write(STDOUT_FILENO, tarstr.gname, strlen(tarstr.gname));
        write(STDOUT_FILENO, " ", 1);
        int sizei;
        sscanf(tarstr.size,"%o",&sizei);
        int sizej=sizei,g=0;
        while(sizej>0){
          sizej=sizej/10;
          g++;
        }
        char finalsize[g];
        sprintf(finalsize, "%d", sizei);
        write(STDOUT_FILENO, finalsize, strlen(finalsize));
        write(STDOUT_FILENO, " ", 1);
        time_t rawtime;
        sscanf(tarstr.mtime,"%lo",&rawtime);
        struct tm  ts;
        char buf[18];
        // Format time, "yyyy-mm-dd hh:mm"
        ts = *localtime(&rawtime);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &ts);
        write(STDOUT_FILENO, buf, 18);
        write(STDOUT_FILENO, " ", 1);
      }
      write(STDOUT_FILENO, &tarstr,m);
      if(isl==1){
        write(STDOUT_FILENO, "\n", 1);
      }else{
        write(STDOUT_FILENO, "  ", 2);
      }
      sscanf(tarstr.size,"%o",&size);
      for(int i=0;i<((size+512-1)/512);i++){
        lus=read(fd,buffer,512);
      }
    }
  }while(lus>0);
  close(fd);
  return 0;
}

int ls_tar_file(char *chemin, char *filename, int isl){
  struct posix_header tarstr;
  int fd = open(chemin,O_RDONLY,0),lus,size;
  const char *rights[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-","rwx", NULL };

  if (fd==-1) {
    perror("This file does not exist");
    exit(EXIT_FAILURE);
  }
  do {
    lus=read(fd, &tarstr, 512);
  }while(cmp(tarstr.name,filename)==-1 && lus>0);
  if(cmp(tarstr.name,filename)==0){
    int m=0;
    while(m<100 && tarstr.name[m]!='\0'){
      m++;
    }
    if(isl==1){
      if(tarstr.typeflag=='0'){
        write(STDOUT_FILENO, "-",1);
      }else if(tarstr.typeflag=='5'){
        write(STDOUT_FILENO, "d",1);
      }
      int v= atoi(tarstr.mode);
      write(STDOUT_FILENO, rights[v/100],3);
      write(STDOUT_FILENO, rights[(v%100)/10],3);
      write(STDOUT_FILENO, rights[(v%100)%10],3);
      write(STDOUT_FILENO, " ", 1);
      write(STDOUT_FILENO, tarstr.uname, strlen(tarstr.uname));
      write(STDOUT_FILENO, "/", 1);
      write(STDOUT_FILENO, tarstr.gname, strlen(tarstr.gname));
      write(STDOUT_FILENO, " ", 1);
      int sizei;
      sscanf(tarstr.size,"%o",&sizei);
      int sizej=sizei,g=0;
      while(sizej>0){
        sizej=sizej/10;
        g++;
      }
      char finalsize[g];
      sprintf(finalsize, "%d", sizei);
      write(STDOUT_FILENO, finalsize, strlen(finalsize));
      write(STDOUT_FILENO, " ", 1);
      time_t rawtime;
      sscanf(tarstr.mtime,"%lo",&rawtime);
      struct tm  ts;
      char buf[18];
      // Format time, "yyyy-mm-dd hh:mm"
      ts = *localtime(&rawtime);
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &ts);
      write(STDOUT_FILENO, buf, 18);
      write(STDOUT_FILENO, " ", 1);
    }
    write(STDOUT_FILENO, &tarstr,m);
    if(isl==1){
      write(STDOUT_FILENO, "\n", 1);
    }else{
      write(STDOUT_FILENO, "  ", 2);
    }
  }
  close(fd);
  return 0;
}


int tsh_ls(int argc, char **argv){
  if(argc==1){//if ls is called alone
      execvp("/bin/ls",argv);
  }else{
    //a occurences of tarballs in argv
    //options equals 0 if an option was called
    //if opts is different than -1 then an option different than 'l' was called
    int a=-1,options=-1,opts=-1;
    for(int i=1;i<argc;i++){
      if(argv[i][0]=='-'){//argv[i] is an option
        options=0;
        int x = 0;
        while(argv[i][x] != '\0'){
          x++;
        }
        for(int j=1;j<x;j++){
          if(argv[i][j]!='l'){
            opts=0;
          }
        }
      }else if(cmp(split(argv[i],1),"echec")==-1 || isTar(argv[i])==1){//argv[i] is a tarball
        a=0;
      }
    }
    if(a==-1){//if there is no tar in argv, we just call ls on argv
      execvp("/bin/ls",argv);
    }else{//if there are tarballs, we need to call ls on each argv[i]: normal ls on regular files, our ls on tarballs
      if(opts!=-1){//options other than l aren't compatible with tarballs
        perror("Only l option works on tarballs");
        exit(EXIT_FAILURE);
      }else{
        for(int i=1;i<argc;i++){
          if(argv[i][0]!='-'){
            if(cmp(split(argv[i],1),"echec")==-1){//tarball
              if(cmp(split(argv[i],1),argv[i])==0 || samepath(argv[i],split(argv[i],1))==0){// if the whole path is a tar
                write(STDOUT_FILENO, split(argv[i],1), strlen(split(argv[i],1)));
                write(STDOUT_FILENO, ":\n", 2);
                if(options==0){
                  ls_tar(argv[i],1);
                }else{
                  ls_tar(argv[i],0);
                }
                write(STDOUT_FILENO, " \n", 2);
              }else{//otherwise, we just use ls on the file specified
                write(STDOUT_FILENO, split(argv[i],1), strlen(split(argv[i],1)));
                write(STDOUT_FILENO, ":\n", 2);
                if(options==0){
                  ls_tar_file(split(argv[i],1),split(argv[i],2),1);
                }else{
                  ls_tar_file(split(argv[i],1),split(argv[i],2),0);
                }
                write(STDOUT_FILENO, " \n", 2);
              }
            }else{//regular file
              write(STDOUT_FILENO, argv[i], strlen(argv[i]));
              write(STDOUT_FILENO, ":\n", 2);
              int pid=fork();
              if(pid<0)
                return (EXIT_FAILURE);
              if(pid==0){
                if(options==0){//options equals 0 if there is an option: in our case only 'l' is the option that made it through the previous "if" clauses
                  execlp("ls","ls","-l",argv[i],NULL);
                }else{
                  execlp("ls","ls",argv[i],NULL);
                }
              }
              waitpid(pid,NULL,0);
            }
          }
        }
      }
    }
  }
	return 0;
}
