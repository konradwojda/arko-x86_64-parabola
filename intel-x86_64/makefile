CC=gcc
CFLAGS= -Wall -rdynamic

all:	main.o draw_bitmap.o
	$(CC) $(CFLAGS) main.o draw_bitmap.o -o parabola `pkg-config --cflags-only-other --libs gtk+-3.0`

main.o:	main.c
	$(CC) $(CFLAGS) `pkg-config --cflags-only-I --libs gtk+-3.0` -O0 -fpack-struct -c main.c -o main.o

draw_bitmap.o:	draw_bitmap.s
	nasm -g -f elf64 draw_bitmap.s

clean:
	rm -f *.o
