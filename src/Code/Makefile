CC=gcc
CFLAGS=-g -Wall
ALL=ls	mycat
all:$(ALL)

ls: ls.o functions.o tar.o
	$(CC) -o ls ls.o functions.o tar.o

ls.o: ls.c functions.h tar.h
	$(CC) -o ls ls.c functions.h tar.h

mycat: mycat.o functions.o tar.o
	$(CC) -o mycat mycat.o functions.o tar.o

mycat.o: mycat.c functions.h tar.h
	$(CC) -o mycat mycat.c functions.h tar.h

functions.o: functions.h tar.h
	$(CC) -o functions.h tar.h

clean:
	rm -rf *~ $(ALL)
