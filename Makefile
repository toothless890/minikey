CFLAGS= -g -Werror
CC=gcc

all: editor.o inputlogic.o minikey.o settings.o 
	$(CC) $(CFLAGS) -o minikey editor.o inputlogic.o minikey.o settings.o 
editor.o: editor.c
	$(CC) $(CFLAGS) -c editor.c
inputlogic.o: inputlogic.c
	$(CC) $(CFLAGS) -c inputlogic.c
minikey.o: minikey.c
	$(CC) $(CFLAGS) -c minikey.c
settings.o: settings.c
	$(CC) $(CFLAGS) -c settings.c
clean:
	@rm editor inputlogic minikey settings  *.o 2>/dev/null || true
