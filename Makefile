CC=gcc
CFLAGS=-Wall -g
all : Code/tsh.o
	gcc Code/tsh.c -lreadline -o tsh
tsh.o : tsh.c headers/tar.h headers/cat.h headers/cd.h headers/cp.h headers/functions.h headers/ls.h headers/rm.h headers/rmdir.h headers/reds.h headers/mkdir.h

clean:
	rm -rf Code/tsh.o tsh
