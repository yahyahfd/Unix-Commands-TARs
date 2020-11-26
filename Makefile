CC=gcc
CFLAGS=-g -Wall
ALL=  ls mycat
all:$(ALL)

ls:
	$(CC) -o src/Code/ls src/Code/ls.c

myrmdir:
	$(CC) -o src/Code/myrmdir src/Code/myrmdir.c

mycat:
	$(CC) -o src/Code/mycat src/Code/mycat.c

clean:
	rm -rf src/Code/ls src/Code/mycat
