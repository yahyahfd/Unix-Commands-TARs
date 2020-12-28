CC=gcc
CFLAGS=-Wall -O
ALL=  ls mycat myrmdir myrm cp cd 
all:$(ALL)

ls.o:
	$(CC) -o src/Code/ls.o src/Code/ls.c $(CFLAGS)

myrmdir.o:
	$(CC) -o src/Code/myrmdir.o src/Code/myrmdir.c $(CFLAGS)

mycat.o:
	$(CC) -o src/Code/mycat.o src/Code/mycat.c $(CFLAGS)

myrm.o:
	$(CC) -o src/Code/myrm.o src/Code/myrm.c $(CFLAGS)

cp.o:
	$(CC) -o src/Code/cp.o src/Code/cp.c $(CFLAGS)

cd.o:
	$(CC) -o src/Code/cd.o src/Code/cd.c $(CFLAGS)

clean:
	rm -f src/Code/*.o core
