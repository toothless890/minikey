#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#define BUFFERSIZE 1024
#define VALIDBUFFERCOUNT 1

// char *dict = "/usr/share/dict/words";
char *dict = "google-10000-english.txt";
char *__restrict readBuffer;
char **validWords;
int validWordindex;
char *overflowString;
int overflowLen;
char *neighborsarray[26] = {"aqz", "bgtvfrcjumnhy", "cfrtgbv", "dex", "edx", "frcvgtby", "gvfrtbc", "hynujmb", "ik", "jumyhnb", "ki", "lo", "mnjuyhb", "njuyhmb", "ol", "p'", "qaz", "rfvbgtc", "swx", "tgbvfrc", "ujmnhyb", "vfrtgbc", "wsx", "xsw", "yujhnmb", "zaq"};
__uint32_t neighborBitmaskArray[26];

// Get a single character from the input without requiring flush and immediatly return
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
__uint32_t bitmask(char letter)
{

    letter = tolower(letter);
    letter -= 97; // now a=1
    return 1 << letter;
}

// turns a string into a bitmask of all unique letters in the string
__uint32_t bitmaskgroup(char *letters)
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

// From a input word, a dictionary fd, and the word length, find a list of candidate words that match its bit signature.
void getValidWords(char *userInput, int fd, int len)
{
    int neighbormask[len];
    lseek(fd, 0, SEEK_SET); // reset the dictionary read offset.
    validWords = (char **)malloc(sizeof(char *) * BUFFERSIZE * VALIDBUFFERCOUNT);
    validWordindex = 0;
    readBuffer = (char *)malloc(sizeof(char) * BUFFERSIZE);
    int readsize = 1;

    overflowLen = 0; // letters leftover after reading the buffer (only read BUFFERSIZE number of characters per iteration)

    for (int x = 0; x < len; x++)
    {
        // just replace ; with p because we use raw ascii values for processing
        if (userInput[x] == ';')
        {
            userInput[x] = 'p';
        }

        // expects a single word, breaks on space
        // TODO: check for spaces at top of read and break into individual words

        neighbormask[x] = neighborBitmaskArray[tolower(userInput[x]) - 97];
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
            char *word = (char *)malloc(sizeof(char) * (x - wordStart + overflowLen));

            for (int i = 0; i < overflowLen; i++)
            {
                word[i] = overflowString[i];
            }
            for (int i = wordStart; i < x; i++)
            {
                word[i - wordStart + overflowLen] = readBuffer[i];
            }
            wordlen = x - wordStart + overflowLen;
            overflowLen = 0;

            // check if the word is overlapping into the next buffered read
            if (x != readsize)
            {
                // save words that match all the constraints to a list (length, actual candidate)
                if (wordlen == len && validWordindex < VALIDBUFFERCOUNT * BUFFERSIZE)
                {
                    if (checkword(word, wordlen, neighbormask))
                    {
                        // printf("%s+%d\n",word, wordlen );
                        validWords[validWordindex] = word;
                        validWordindex++;
                    }
                }
            }
            else
            {
                overflowString = word;
                overflowLen = x - wordStart;
            }

            x++;
        }
    }
}

int main()
{
    int fd = open(dict, O_RDONLY);
    int len = 0;
    int candidateIndex = 0;

    // take a human readable array of key neighbors and turn it into a bitmask
    for (int x = 0; x < 26; x++)
    {
        neighborBitmaskArray[x] = bitmaskgroup(neighborsarray[x]);
    }

    // loop on user input
    while (1)
    {
        char userInput[64]; // max word length
        char c = getch();

        switch (c)
        {
        // backspace
        case 127:
            if (len > 0)
            {
                printf("\b \b");
                userInput[--len] = 0;
            }
            break;
        // newline
        case '\n':
            // enter ends a word, or starts a new line if no word is started
            if (len == 0)
            {
                printf("\n");
            }
            else
            {
                len = 0;
                printf(" ");
            }
            break;
        // space
        case ' ':
            // cycle through and print the candiate at index candidateIndex
            getValidWords(userInput, fd, len);
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
            break;

        // standard characters
        default:
            printf("%c", c);
            userInput[len++] = c;
            candidateIndex = 0;
        }
    }
    close(fd);
}
