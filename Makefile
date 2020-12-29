CC=gcc
CFLAGS=-Wall -g
all : Code/tsh.o
	gcc Code/tsh.c -lreadline -o tsh
tsh.o : tsh.c headers/tar.h headers/cat.h headers/cd.h headers/cp.h headers/functions.h headers/ls.h headers/tm.h headers/rmdir.h

cleanall:
	rm -rf Code/*.o *~
clean:
	rm -rf *~
