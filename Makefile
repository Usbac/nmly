CC=gcc
CFLAGS=-lm -std=c99 -pedantic -g
PREFIX=/usr/local
FILES=src/nmly.c src/helper.c src/string.c
ifndef NAME
    NAME=nmly
endif

all: nmly

nmly: $(FILES)
	$(CC) -o $(NAME) $(FILES) $(CFLAGS)

install: nmly
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/$(NAME)

clean:
	rm $(NAME)
