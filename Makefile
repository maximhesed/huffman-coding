CC=gcc

CFLAGS=-Wall -g
LDFLAGS=-lncurses

OBJS=main.o node.c code.c

PROGNAME=prog

all: $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROGNAME) $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -f *.o $(PROGNAME)
