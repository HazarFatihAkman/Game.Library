#ifndef GAME_LIBRARY_FOLDER_H
#define GAME_LIBRARY_FOLDER_H

#include "list.h"

#include <stdio.h>

#define DEFAULT_FOLDER "Games\\*.*"
#define DEFAULT_FOLDER_FORMAT_VERSION "Games"
#define DEFAULT_COVER_FORMAT "covers/%s.jpg"

typedef struct file {
  char *url;
  char *src;
  char *name;
} file_t;

list_t *load_folder();

#endif // GAME_LIBRARY_FOLDER_H

