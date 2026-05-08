#define settings_filename "settings.txt"

typedef struct Settings {
    char *neighborsarray[26];
} settings_t;

settings_t *readSettings();