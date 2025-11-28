#ifndef GAME_LIBRARY_GUI_H
#define GAME_LIBRARY_GUI_H

#define DEFAULT_H 1280 
#define DEFAULT_W  720

#define CSS_FILE_PATH "style.css"

#include "../include/folder.h"

#include <gtk/gtk.h>

typedef struct display_component {
  GtkWidget *label;
  GtkWidget **games;
  int game_size;
} display_component_t;

GtkWidget *create_label(int total_games_count);
GtkWidget *create_btn(file_t *file);

#endif // GAME_LIBRARY_GUI_H

