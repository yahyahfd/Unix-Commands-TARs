CC=gcc
CFLAGS=-g -Wall
ALL=ls mycat
all:$(ALL)

ls: src/Code/ls.c
	$(CC) -o src/Compile/ls src/Code/ls.c

mycat: src/Code/mycat.c
	$(CC) -o src/Compile/mycat src/Code/mycat.c

clean:
	rm -rf src/Compile/*
