CFLAGS= -g -Werror
CC=gcc

all: ansiCodes.o editor.o keylogic.o minikey.o settings.o 
	$(CC) $(CFLAGS) -o minikey ansiCodes.o editor.o keylogic.o minikey.o settings.o 
ansiCodes.o: ansiCodes.c
	$(CC) $(CFLAGS) -c ansiCodes.c
editor.o: editor.c
	$(CC) $(CFLAGS) -c editor.c
keylogic.o: keylogic.c
	$(CC) $(CFLAGS) -c keylogic.c
minikey.o: minikey.c
	$(CC) $(CFLAGS) -c minikey.c
settings.o: settings.c
	$(CC) $(CFLAGS) -c settings.c
clean:
	@rm ansiCodes editor keylogic minikey  settings  *.o 2>/dev/null || true
