#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "settings.h"

char **parseFields(char *line) {
    char **setting = malloc(2 * sizeof(char *));
    char *field = line;
    line[strcspn(line, "\r\n")] = 0;
    while ((*field) != ':') {
        field++;
    }
    *(field++) = '\0';

    char *data = (field + 1);

    setting[0] = line;
    setting[1] = data;
    return setting;
}

settings_t *readSettings() {
    settings_t *settings = (settings_t *) malloc(sizeof(settings_t));

    FILE *fp = fopen("settings.txt", "r+");
    
    char line[64];
    char field[32];
    char data[32];

    while (fgets(line, sizeof(line), fp) != NULL) {
        char **setting = parseFields(line);
        if (strcmp(setting[0], "neighborsarray") == 0) {
            // TODO: read neighbors
        }
        
    }
    return settings;
}

int main(void) {
    readSettings();
}