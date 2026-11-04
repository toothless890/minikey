#include "editor.h"
#include "ansiCodes.h"
#define UNDOBUFFERSIZE 256


int terminalWidth;
int terminalHeight;
int cursorX;
int cursorY;
char* filename;
char** lineBufferArray; // this is not super scalable, but for smallish text documents, it should be fine
int* lineSizeArray;
// fields:
// filename (char*)
// linebufferarray (char **)
// linesizearray (int*)

// undobuffer (???) + UNDOBUFFERSIZE
// undoindex (int)

//cursorX (int)
//cursorY (int)

// viewportY (int) index of the first line to display (for files too big to display on one screen, increases when down is applied on the bottom line, decreases when up is applied on top line when top line is not 0)
// VIEWPORTWIDTH (int)
// VIEWPORTHEIGHT (int)

//selectedWordX (int)
//selectedWordY (int)



// methods:

// read() reads file into line buffers returns 0 on success or -1 failure 

// save() writes line buffers back into the original file
// close() closes file descriptor, frees everything, in prep for exit

// fetchword() (returns the current selected word for use in keylogic)
// typechar(char) (types in a pressed key at the cursor and advances it accordingly (forward if a normal key, backwards if a backspace). updates visual display of specific buffer)
// deleteSelectedWord() removes selected word by re-writing the buffer, updates visual display of that buffer

// navigate(int (0-3 directional up down left right)) (moves cursor, highlighting the relevant word)
// undo() (returns 0 on success, -1 on failure, 1 on end of buffer)
// redo() (returns 0 on success, -1 on failure, 1 on end of buffer)

// drawAllBuffers() completely clears and redraws the terminal


void initEditor(){
    signal(SIGWINCH, resizeTerminal);
    zeroCursorXY();
    resizeTerminal(0);
}



void resizeTerminal(int signal){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    terminalHeight = w.ws_row;
    terminalWidth = w.ws_row;
    clearTerminal();
    printf("\rResized! Rows: %d, Cols: %d   ", terminalHeight, terminalWidth);
    fflush(stdout);
}

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
