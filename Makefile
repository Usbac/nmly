CC=gcc
CFLAGS=-lm -std=c99 -pedantic -g
LDFLAGS=
PREFIX=/usr/local
FILES=src/helper.c src/string.c
ifndef NAME
    NAME=nmly
endif

all: nmly

nmly: $(FILES)
	$(CC) -o $(NAME) $(FILES) src/nmly.c $(CFLAGS) $(LDFLAGS)

install: nmly
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/$(NAME)

test:
	$(CC) -o nmly_test $(FILES) tests/main.c $(CFLAGS)

clean:
	rm $(NAME)
