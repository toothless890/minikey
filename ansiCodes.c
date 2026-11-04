#include "ansiCodes.h"

void clearTerminal(){
    printf("\e[2J");
}

void zeroCursorXY(){
    printf("\e[H");
    // printf("\e[1;1H\e[2J");
}
// void zeroCursorX(){
//     printf("\e[0F");

// }

// void saveScreen(){
//     printf("\e[s\e[?47h");

// }

// void restoreScreen(){
//     printf("\e[?47l\e[u");

// }

void saveScreen(){
    printf("\033[?1049h");

}

void restoreScreen(){
    printf("\033[?1049l");

}

