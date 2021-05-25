CC=gcc
#CFLAGS= -Wall -rdynamic

all:	main.o functions.o
	$(CC) -Wall -rdynamic main.o functions.o -o fun `pkg-config --cflags-only-other --libs gtk+-3.0`

main.o:	main.c
	$(CC) -Wall -rdynamic `pkg-config --cflags-only-I --libs gtk+-3.0` -O0 -fpack-struct -c main.c -o main.o

functions.o:	functions.s
	nasm -g -f elf64 functions.s

clean:
	rm -f *.o
