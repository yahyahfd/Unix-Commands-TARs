CC=gcc
CFLAGS=-g -Wall
ALL=  ls mycat myrmdir myrm cp cd reds exit
all:$(ALL)

ls:
	$(CC) ls.c -o ls
myrmdir:
	$(CC)  myrmdir.c -o myrmdir

mycat:
	$(CC) mycat.c -o mycat
myrm :
	$(CC) myrm.c -o myrm
cd:
	$(CC)  cd.c -o cd
cp :
	$(CC) cp.c -o cp
reds:
	$(CC) reds.c -o reds
exit :
	$(CC) exit.c -o exit
mv:
	$(CC) mv.c -o mv
clean:
	rm -f ls mycat myrm myrmdir cp cd exit reds mv
