
#define BUFFERSIZE 1024


char getch();
int bitmask(char letter);
int bitmaskgroup(char *letters);
int checkword(char *word, int len, int *neighbormask);
int clearMemory(char **wordlist, int len);