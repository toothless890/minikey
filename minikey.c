#include "editor.h"
#include "keylogic.h"
#include "ansiCodes.h"

int main(int argv, char **argc){
    saveScreen();
    initEditor();
    char c = '\0';

    // load settings file into minikey, editor
    // read file (open and load contents into buffer)
    // begin main loop
    while(c != 1){
        c = getch();
        printf("%c", c);
        sleep(0);
        // while not exiting...
        // handle inputs (navigation, editing, saving, exiting...)
            // all file buffers are stored in editor
            // 
    // exit loop on exit input
    }
    restoreScreen();
    // close file, exit
    return 0;
}