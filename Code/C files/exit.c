#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int exit2(char **args){
	exit(EXIT_SUCCESS);
	}

int main(char **args){
	exit2(args);
	}
