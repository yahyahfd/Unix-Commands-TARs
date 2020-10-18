#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

		
/***************la fonction cat*****************/
	
int main(int argc, char **argv){

	int fd;
	int lus;
	char buffer[1000];
	
	if(argc<2){
		while(lus =read(0, buffer ,sizeof(buffer)))
			write(1 ,buffer ,lus);
	}
	if(isTar(argv[1])==1){
		
		for(int i=2;i<argc;i++){
			contenu_tar(argv[1],argv[i]);
			}
		}
	else{	
			
		for(int i=1;i<argc;i++){
			
			fd=open(argv[i] , O_RDONLY);
			
			if(fd == -1)
				perror("Cannot open file");
				
			else{
				
				while(lus =read(fd, buffer ,sizeof(buffer)))
					write(STDOUT_FILENO ,buffer ,lus);
					
				close(fd);
				}
			}
		}
	return 0;
}
