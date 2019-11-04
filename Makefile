CC=gcc
CFLAGS=-ansi

all: nmly

nmly: nmly.o helper.o
	gcc -v -o nmly helper.o nmly.o

helper.o: helper.c
	gcc -v -c helper.c

nmly.o: nmly.c
	gcc -v -c nmly.c

clean:
	rm *.o nmly
