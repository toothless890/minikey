CFLAGS= -g -Werror
CC=gcc

all: minikey.o
	$(CC) $(CFLAGS) -o minikey minikey.o

minkey.o: minikey.c
	$(CC) $(CFLAGS) -c minikey.c

run: all
	./minikey

clean:
	@rm minikey *.o 2>/dev/null || true
