CC=gcc
CFLAGS=-lm -std=c99 -pedantic
PREFIX=/usr/local
ifndef NAME
    NAME=nmly
endif

all: nmly

nmly: src/nmly.c src/helper.c
	$(CC) -o $(NAME) src/nmly.c src/helper.c $(CFLAGS)

install: nmly
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/$(NAME)

clean:
	rm $(NAME)
