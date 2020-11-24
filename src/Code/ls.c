#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include "functions.h"

int ls(char *dir, int isl){
  DIR *dirp;
  struct dirent *entry;
  const char *rights[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-","rwx", NULL };
  int right;
  int b;
	struct stat sb;

	if(stat(dir, &sb)==-1){
		perror("stat");
		exit(1);
	}

	if(S_ISDIR(sb.st_mode)){ //dir is a directory
	  if((dirp = opendir(dir)) == NULL) {//opening directory
	    perror("Error while opening the directory ");
	    exit(EXIT_FAILURE);
	  }
	  while((entry=readdir(dirp))){//printing each file/folder name inside dir
      if(isl==0){
        printf("%s\n", entry->d_name);
      }else{
        b = lstat (entry->d_name, &sb);
				if (b == -1){
          printf ("Inexistant path\n");
        }else {
          printf ("%s", rights[(right>>6)&7] ); //Owner
          printf ("%s", rights[(right>>3)&7] ); //Group
          printf ("%s", rights[(right)&7] ); //Others
          printf (" %s\n",entry->d_name);
          right = sb.st_mode;
        }
	    }
    }
		if(closedir(dirp) == -1) {//closing directory
			perror("Error while closing the directory");
			exit(EXIT_FAILURE);
		}
	}else{ //dir is a file
    if(isTar(dir)){
      struct posix_header p;
      int fd=open(dir,O_RDONLY),i=0,lus;

      while(lus>0){
        lus=read(fd,&p,512);
        printf("%s\n", p.name);
        int x = (atoi(p.size)-512+1)/512;
        do {
          lus=read(fd,&p,512);
          i++;
        }
        while(i<x);
      }
      if(close(fd) == -1) {//closing directory
        perror("Error while closing the tarball ");
        exit(EXIT_FAILURE);
      }
    }else{
  		printf("%s\n", dir);
    }
	}
}

int main(int argc, char **argv){
  if(argc==1){//if ls is called alone
      execvp("/bin/ls",argv);
  }else{
    //a occurences of tarballs in argv
    //options equals 0 if an option was called
    //if opts is different than -1 then an option different than 'l' was called
    int a=0,options=-1,opts=-1;
    for(int i=1;i<argc;i++){
      if(argv[i][0]=='-'){//argv[i] is an option
        options=0;
        int x = 0;
        while(argv[i][x] != '\0'){
          x++;
        }
        for(int j=1;j<x;j++){
          if(argv[i][j]!='l'){
            opts++;
          }
        }
      }else if(cmp(split(argv[i],1),"echec")==-1 || isTar(argv[i])==1){//argv[i] is a tarball
        a++;
      }
    }
    if(a==0){//if there is no tar in argv, we just call ls on argv
      execvp("/bin/ls",argv);
    }else{//if there are tarballs, we need to call ls on each argv[i]: normal ls on regular files, our ls on tarballs
      if(opts!=-1){//options other than l aren't compatible with tarballs
        perror("Only l option works on tarballs");
        exit(EXIT_FAILURE);
      }else{
        for(int i=1;i<argc;i++){
          if(argv[i][0]!='-'){
            if(cmp(split(argv[i],1),"echec")==-1){//tarball
              if(cmp(split(argv[i],2),NULL)==-1){// if the whole path is a tar

              }else{//otherwise, we just use ls on the file specified

              }
            }else{//regular file
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

  /*
  if(argc==1){//if ls is called alone
      execvp("/bin/ls",argv);
  }else if(argc==2 && argv[1][0]=='-'){//if ls is called with an option
    execvp("/bin/ls",argv);
  }else{//if ls is called on a file, folder, tarball... (with or without option)
    //First we check if there are any options
    int i=1,option=-1,optl=-1;
    while(i<argc){
      if(argv[i][0]=='-' ){//there are options
        option=0;
        if(argv[i][1]=='l'){
          printf("%ld\n", sizeof(argv[i])/sizeof(argv[i][0]));
          optl++;//the only option is l if optl equals 0
        }
      }
      i++;
    }
  }

    if(option==0){ //We check whether there are options or not...
      for(int i=1;i<argc;i++){
        if(cmp(splitbis(argv[i],1),splitbis(argv[i],2))==-1 && argv[i][0]!='-'){//... and if argv[i] is a tarball
          if(optl==0){//this is only true if the only option is l
            printf("tarball with option l\n");
          }else if(optl==-1){
            printf("tarball with no option\n");
          }else{
            perror("Only the option l is supported with tarballs");
            exit(EXIT_FAILURE);
          }
        }else{//
          if(argv[i][0]!='-'){
            printf("gj\n");
          }
        }
      }
    }else{
      execvp("/bin/ls",argv);
    }
  }




/*
  //First, we check whether there is a tarball or not
  for(int i=1;i<argc;i++){
    if(cmp(splitbis(argv[i],1),splitbis(argv[i],2))==0){//argv[i] is a tarball
      //The only option supported for a tar is the "-l" one, we check if there is an option different than this one
      int i=1, option=-1;
      while(i<argc){//if there is a "-l" option, "option" becomes equal to 0
      	if(argv[i][0]=='-' ){

      	}
    		i++;
    	}
      //show what's inside the tar
    }else{//argv[i] is a regular file/directory
      //We check again if there are any options

        int pid=fork();
        if(pid<0)
          return (EXIT_FAILURE);
        if(pid==0){
            execlp("ls","ls",argv[i],NULL);
          }
          waitpid(pid,NULL,0);

    }
  }
*/


/*
	if(argc==1){ //"ls" called alone
		ls(".",0);
	}else if(argc==2){ //"ls -l" or "ls directory_path" or "ls file_path" or "ls tar"
		if((cmp(argv[1], "-l") == 0)){
      ls(".",1);
		}else{
			ls(argv[1],0);
		}
	}else if(argc==3){ //"ls -l +  directory_path or file_path"
		if((cmp(argv[1], "-l") == 0)){
      ls(argv[2],1);
		}else{ //syntax error
			fprintf(stderr, "Erreur de syntaxe.\n");
			exit(EXIT_FAILURE);
		}
	}else{ //syntax error
		fprintf(stderr, "Erreur de syntaxe.\n");
		exit(EXIT_FAILURE);
	}
*/
	return 0;
}
