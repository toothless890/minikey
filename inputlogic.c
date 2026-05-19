#include <termios.h>
#include <unistd.h>
#include "inputlogic.h"
// Get a single character from the input without requiring flush and immediatly return
char *dict = "extended-dictionary.txt";


char getch()
{
    int c;
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    newt.c_lflag &= ~(ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}

// turns a letter into a bitmask representation -- a = 1, b = 2, c = 4, d = 8 etc.
int bitmask(char letter)
{
    letter = tolower(letter);
    // TODO: this does not properly work if a character is not a letter. It is functionally capable, just not working as planned
    letter -= 97; // now a=1
    return 1 << letter;
}

// turns a string into a bitmask of all unique letters in the string
int bitmaskgroup(char *letters)
{
    int len = strlen(letters);
    int result = 0;
    for (int x = 0; x < len; x++)
    {
        result |= bitmask(letters[x]);
    }
    return result;
}
// compares a word against a list of bitmasks(each one representing the candidate letters for that index) and return true
// if and only if all of the letters in the word are candidates
int checkword(char *word, int len, int *neighbormask)
{
    int result = 1;
    for (int x = 0; x < len; x++)
    {
        result &= ((bitmask(word[x]) & neighbormask[x]) != 0);
    }
    return result;
}


int clearMemory(char **wordlist, int len)
{
    for (int i = 0; i < len; i++)
    {
        free(wordlist[i]);
    }
}

// From a input word, a dictionary fd, and the word length, find a list of candidate words that match its bit signature.
void getValidWords(char *userInput, int fd, int pos, int len)
{
    
    int neighbormask[len];
    validWordindex = 0;

    lseek(fd, 0, SEEK_SET); // reset the dictionary read offset.

    int readsize = 1;

    overflowLen = 0; // letters leftover after reading the buffer (only read BUFFERSIZE number of characters per iteration)

    for (int x = 0; x < len; x++)
    {
        // just replace ; with p because we use raw ascii values for processing
        if (userInput[x] == ';')
        {
            userInput[x] = 'p';
        }

        neighbormask[x] = neighborBitmaskArray[tolower(userInput[x]) - 97];
        // printf("%d ",neighbormask[x]);
    }

    // loop untill eof
    while (readsize > 0)
    {
        readsize = read(fd, readBuffer, BUFFERSIZE);
        // printf("%d\n",readsize);
        int x = 0;
        int wordStart = 0;
        // loop untill end of buffer
        while (x < readsize)
        {
            wordStart = x;
            // loop until end of word
            while (x < readsize && readBuffer[x] != '\n')
            {
                x++;
            }

            int wordlen;
            wordlen = x - wordStart + overflowLen;
            
            char *word = (char *) malloc(sizeof(char) * (wordlen+1));

            for (int i = 0; i < overflowLen; i++)
            {
                word[i] = overflowString[i];
            }
            for (int i = wordStart; i < x; i++)
            {
                word[i - wordStart + overflowLen] = readBuffer[i];
            }
            overflowLen = 0;

            // check if the word is overlapping into the next buffered read
            if (x != readsize)
            {
                // save words that match all the constraints to a list (length, actual candidate)
                if (wordlen == len && validWordindex < BUFFERSIZE)
                {
                    if (checkword(word, wordlen, neighbormask))
                    {
                        // printf("%c ", word[wordlen]);
                        // printf("%ld ",sizeof(word));
                        word[wordlen] = '\0';
                        // printf("%s+%d\n",word, wordlen );
                        validWords[validWordindex] = word;
                        validWordindex++;
                    }
                }
            }
            else
            {
                overflowString = word;
                overflowLen = readsize-x;
            }
            x++;
        }
    }
}
