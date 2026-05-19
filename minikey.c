#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "inputlogic.h"

// char *dict = "/usr/share/dict/words";
// TODO: make the dictionary swappable if the 10k words are unsatisfactory
// char *dict = "google-10000-english.txt";

char **validWords;
int validWordindex;
char *overflowString;
int overflowLen;


int main()
{
    int fd = open(dict, O_RDONLY);
    int len = 0;
    int wordstart = 0;
    int candidateIndex = 0;
    int newWordFlag = 0;

    readBuffer = (char *)malloc(sizeof(char) * BUFFERSIZE);
    validWords = (char **)malloc(sizeof(char *) * BUFFERSIZE);

    // take a human readable array of key neighbors and turn it into a bitmask
    for (int x = 0; x < 26; x++)
    {
        neighborBitmaskArray[x] = bitmaskgroup(neighborsarray[x]);
    }

    // loop on user input
    while (1)
    {
        char userInput[BUFFERSIZE]; // max word length
        char c = getch();

        switch (c)
        {
        // backspace
        case 127:
            if (wordstart > 0 || len > 0)
            {
                printf("\b \b");
                if (len == 0)
                {
                    wordstart--;

                }
                else
                {
                    len--;
                }
                userInput[wordstart + len] = 0;
            }
            newWordFlag = 0;
            break;
        // newline
        case '\n':
            newWordFlag = 0;
            // enter ends a word, or starts a new line if no word is started
            if (len == 0)
            {
                // at this moment, the userInput buffer contains the whole line of desired text. This can be used as an output.
                wordstart = 0;
                printf("\n");
            }
            else
            {
                // start a new word without whitespace
                len = 0;
            }
            break;
        // space
        case ' ':
            // cycle through and print the candiate at index candidateIndex
            if (!newWordFlag)
            {
                clearMemory(validWords, validWordindex);
                getValidWords(userInput, fd, wordstart, len);
            }
            // delete the last len letters from the preview
            if (validWordindex > 0)
            {
                for (int i = 0; i < len; i++)
                {
                    printf("\b");
                }
                int wordlen = printf("%s", validWords[candidateIndex % validWordindex]);
                candidateIndex++;
            }
            newWordFlag = 1;
            break;

        // standard characters
        default:
            if (newWordFlag == 1)
            {
                printf(" ");
                // printf("clearing memory from space");
                // clearMemory(validWords, validWordindex);
                wordstart += len+1;
                len = 0;
            }
            printf("%c", c);
            userInput[len++] = c;
            candidateIndex = 0;
            newWordFlag = 0;
        }
    }
    close(fd);
    free(readBuffer);
    free(validWords);
}
