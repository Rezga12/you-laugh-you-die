CC=gcc -pthread
FLAGS=`pkg-config fuse3 --cflags --libs`


#main:
#	gcc chache.c main.c `pkg-config fuse3 --cflags --libs` -o cachefs

all : main.o chache.o inode.o directory.o list.o file.o
	$(CC) -o cachefs main.o chache.o inode.o directory.o list.o file.o $(FLAGS)


main.o : main.c 
	$(CC) -c main.c $(FLAGS)

chache.o : chache.c chache.h
	$(CC) -c chache.c $(FLAGS)

inode.o : inode.c inode.h
	$(CC) -c inode.c $(FLAGS)

directory.o : directory.c directory.h
	$(CC) -c directory.c $(FLAGS)

list.o : list.c list.h
	$(CC) -c list.c $(FLAGS)

file.o : file.c file.h
	$(CC) -c file.c $(FLAGS)

clean :
	rm cachefs main.o chache.o inode.o directory.o list.o file.o
