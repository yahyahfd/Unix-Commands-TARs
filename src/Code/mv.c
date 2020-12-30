#include "functions.h"
#include <string.h>

int mv_tar (char *filepath, char *destination, int nb){
  if(cmp(split(filepath,1),"echec") == -1){
    if(cmp(split(destination,1),"echec") == -1){//from tar to tar
      int fd=open(split(filepath,1),O_RDWR);
      if(fd==-1){
        perror("Cannot open this file");
        exit(EXIT_FAILURE);
      }
      int fdbis=open(split(destination,1),O_RDWR);
      if(fdbis==-1){
        perror("Cannot open this file");
        exit(EXIT_FAILURE);
      }

      int lus;
      struct posix_header p;
      //we first check if filepath exists
      do {
        lus=read(fd,&p,512);
      }
      while(cmp(p.name,split(filepath,2))==-1 && lus>0);
      if(cmp(p.name,split(filepath,2))==0){//filepath exists
        //we now check if destination exists
        int lusbis;
        struct posix_header pbis;
        do {
          lusbis=read(fdbis,&pbis,512);
        }
        while(cmp(pbis.name,split(destination,2))==-1 && lusbis>0);
        if(cmp(pbis.name,split(destination,2))==0){//destination exists, we replace it with filepath
          if(p.typeflag==5){ //it's a directory
            if(pbis.typeflag==5){//pbis is a directory too
              if(nb==0){//we replace if directory exists, or rename otherwise
                strcpy(pbis.name,p.name);
                strcpy(pbis.mode,p.mode);
                strcpy(pbis.uid,p.uid);
                strcpy(pbis.gid,p.gid);
                strcpy(pbis.size,p.size);
                strcpy(pbis.mtime,p.mtime);
                strcpy(pbis.linkname,p.linkname);
                strcpy(pbis.magic,p.magic);
                strcpy(pbis.version,p.version);
                strcpy(pbis.uname,p.uname);
                strcpy(pbis.gname,p.gname);
                strcpy(pbis.devmajor,p.devmajor);
                strcpy(pbis.devminor,p.devminor);
                strcpy(pbis.prefix,p.prefix);
                strcpy(pbis.junk,p.junk);
                set_checksum(&pbis);
              }else{//we move all args inside the target directory

              }
            }else{
              perror("Mv : You cannot move a directory into this typeflag");
              exit(-1);
            }
          }else if(p.typeflag==0){ //it's a regular file
            if(pbis.typeflag==0){ //pbis is a regular file too
              if(nb==0){ // we replace the target file with the regular one
                strcpy(pbis.name,p.name);
                strcpy(pbis.mode,p.mode);
                strcpy(pbis.uid,p.uid);
                strcpy(pbis.gid,p.gid);
                strcpy(pbis.size,p.size);
                strcpy(pbis.mtime,p.mtime);
                strcpy(pbis.linkname,p.linkname);
                strcpy(pbis.magic,p.magic);
                strcpy(pbis.version,p.version);
                strcpy(pbis.uname,p.uname);
                strcpy(pbis.gname,p.gname);
                strcpy(pbis.devmajor,p.devmajor);
                strcpy(pbis.devminor,p.devminor);
                strcpy(pbis.prefix,p.prefix);
                strcpy(pbis.junk,p.junk);
                set_checksum(&pbis);
              }else{
                perror("Mv : You cannot move more than a file into a file(replacing)");
                exit(-1);
              }
            }else if(pbis.typeflag==5){//we move the file into the directory

            }else{
              perror("Mv : You cannot move a file into this typeflag");
              exit(-1);
            }
          }else{
            perror("Mv : This typeflag is not handled");
            exit(-1);
          }
        }else{//we just rename and move filepath
          strcpy(p.name,pbis.name);
        }
      }

      close(fd);
      close(fdbis);
    }else{//tar to dir
      int fd=open(split(filepath,1),O_RDWR);
      if(fd==-1){
        perror("Cannot open this file");
        exit(EXIT_FAILURE);
      }

      int lus;
      struct posix_header p;
      //we first check if filepath exists
      do {
        lus=read(fd,&p,512);
      }
      while(cmp(p.name,split(filepath,2))==-1 && lus>0);
      if(cmp(p.name,split(filepath,2))==0){//filepath exists
        struct stat s;
        int err = stat(destination, &s);
        if(err == -1) {
            if(ENOENT == errno) {// does not exist, we simply move filepath to destination

            }else{
                perror("stat");
                exit(1);
            }
        } else {
            if(S_ISDIR(s.st_mode)) {// destination is a directory
              if(p.typeflag==5){//we move a directory into a directory

              }else if(p.typeflag==0){//we move a file into a directory

              }else{//anything else cannot be moved into a directory in our case
                perror("Mv : You cannot move a directory into this typeflag");
                exit(EXIT_FAILURE);
              }
            }else if(S_ISREG(s.st_mode)){ // destination is a file in our case
              if(p.typeflag==5){//cannot move a directory into a file
                perror("Mv : You cannot move a directory into this typeflag");
                exit(EXIT_FAILURE);
              }else if(p.typeflag==0){//we rename the file if this file exists, or just move it

              }else{//anything else cannot be moved into a directory in our case
                perror("Mv : You cannot move a directory into this typeflag");
                exit(EXIT_FAILURE);
              }
            }else{//destination is nor a directory nor a file
              perror("Mv : You cannot move a directory into this typeflag");
              exit(EXIT_FAILURE);
            }
        }
      }
      close(fd);
    }
  }else{
    if(cmp(split(destination,1),"echec") == -1){//dir to tar

    }
    //this fonction is never called when trying to move a dir to a dir
  }
  return 0;
}

int main(int argc, char **argv){
  //We start by checking if there is any tarball inside arguments, and we check if there are any options (no options compatible for tarballs)
  int tmp = -1;
  int options = -1;
  for(int i=1;i<argc;i++){
    if(cmp(split(argv[i],1),"echec") == -1 && cmp(split(argv[i],1),argv[i])==-1){
      tmp=0; //there is at least a tarball inside arguments
    }
    if(argv[i][0]=='-'){//argv[i] is an option
      options=0;
    }
  }
  if(tmp==0){
    if(options==0){
      perror("Mv : No option compatible with mv");
      exit(-1);
    }else{//Work on tarball cases
      if(argc==3){// only two arguments, we simply call mv_tar on the two last arguments (since the first one is mv)
        mv_tar(argv[1],argv[2],0);
      }else if(argc>3){//if we have more than two arguments, we need to check if the last one is a directory
        if(cmp(split(argv[argc-1],1),"echec") == -1){ //this is a tarball
          int a=-1;
          if(isTar(argv[argc-1])){
            a=0;
          }
          int fd=open(split(argv[argc-1],1),O_RDWR);
          if(fd==-1){
            perror("Cannot open this file");
            exit(EXIT_FAILURE);
          }
          int lus;
          struct posix_header p;
          do {
            lus=read(fd,&p,512);
          }
          while(cmp(p.name,split(argv[argc-1],2))==-1 && lus>0);
          if(cmp(p.name,split(argv[argc-1],2))==0){
            if (p.typeflag==5) {
              a=0;
            }
          }
          if(a==0){
            for(int i=1;i<(argc-1);i++){
              mv_tar(argv[i],argv[argc-1],1);
            }
          }
        }else{// we check if it's a directory directly
          struct stat s;
          int err = stat(argv[argc-1], &s);
          if(err == -1) {// does not exist
              if(ENOENT == errno) {
                perror("Mv : Destination directory doesn't exist");
                exit(-1);
              }else{
                perror("stat");
                exit(1);
              }
          } else {
            if(S_ISDIR(s.st_mode)){// destination is a directory
              for(int i=1;i<(argc-1);i++){
                mv_tar(argv[i],argv[argc-1],1);
              }
            }
          }
        }
      }
    }
  }else{//There is no Tarball Involved, we just call the mv function as it is on all arguments (including the options)
    for(int i=1;i<argc;i++){
      int pid=fork();
      if(pid<0)
        return (EXIT_FAILURE);
      if(pid==0)
        execlp("mv","mv",argv[i],NULL);
      waitpid(pid,NULL,0);
    }
  }
  return 0;
}
