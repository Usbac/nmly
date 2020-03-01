CC=gcc
CFLAGS=-ansi -g

all: nmly

nmly: src/nmly.o src/helper.o
	gcc -v -o nmly src/helper.o src/nmly.o $(CFLAGS)

helper.o: src/helper.c
	gcc -v -c src/helper.c

nmly.o: src/nmly.c
	gcc -v -c src/nmly.c

clean:
	rm src/*.o nmly
