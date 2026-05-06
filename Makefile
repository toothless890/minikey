CFLAGS= -g -Werror
CC=gcc

all: minkey.o
	$(CC) $(CFLAGS) -o minikey minkey.o

minkey.o: minkey.c
	$(CC) $(CFLAGS) -c minkey.c

run: all
	./minikey

clean:
	@rm minikey *.o 2>/dev/null || true
