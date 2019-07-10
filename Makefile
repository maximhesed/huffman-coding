CC=gcc

CFLAGS=-c -Wall -g -O0
LDFLAGS=-lncurses

OBJS=main.o
DEPS=node.h

PROGNAME=prog

all: $(OBJS) $(DEPS)
	$(CC) $(LDFLAGS) -o $(PROGNAME) $^

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f *.o $(PROGNAME)
