#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <windows.h>
#include <shellapi.h>

#include "../include/gui.h"
#include "../include/utils.h"
 
GtkWidget *create_label(int total_game_count) {
  fprintf(stderr, "[Info] : Label creating...\n");

  const int text_len = 28;
  char text[text_len];
  const char *game_text = "No Game Found"; 
  if (total_game_count == 0) {
    snprintf(text, text_len, "Total Games : %s", game_text);
  }
  else {
    snprintf(text, text_len, "Total Games : %d", total_game_count);
  }

  fprintf(stderr, "[Info] : Label text -> %s\n", text);

  GtkWidget *label = gtk_label_new(text);
  gtk_widget_add_css_class(label, "total-counter");

  fprintf(stderr, "[Info] : Label created.\n");
  return label;
}

static void shell_execute_path(const char *path) {
     INT_PTR result = (INT_PTR)ShellExecuteA(
        NULL,
        "open",
        path,
        NULL,
        NULL,
        SW_SHOWNORMAL    );

    if (result <= 32) {
        fprintf(stderr, "ShellExecute Error (%s): Failed with code %ld\n", path, (long)result);
    }
}

static void btn_click_event(GtkButton *btn, gpointer in) {
    const char *temp_path = (char*) in;
    char *full_path = NULL;

    if (strstr(temp_path, ".lnk")) {
        full_path = g_build_filename(DEFAULT_FOLDER_FORMAT_VERSION, temp_path, NULL);
    } else {
        full_path = g_strdup(temp_path);
    }

    shell_execute_path(full_path);

    g_free(full_path);
}

GtkWidget *create_btn(file_t *file) {
  GtkWidget *btn = gtk_button_new();

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *image = gtk_image_new();

  gchar *base_name_utf8 = g_locale_to_utf8(file->name, -1, NULL, NULL, NULL);
  GtkWidget *label = gtk_label_new(base_name_utf8);

  gtk_image_set_from_file(GTK_IMAGE(image), file->src);
  if (gtk_image_get_storage_type(GTK_IMAGE(image)) == GTK_IMAGE_EMPTY) { 
    gtk_widget_add_css_class(image, "game-cover");
  }

  gtk_box_append(GTK_BOX(box), image);
  gtk_box_append(GTK_BOX(box), label);

  gtk_button_set_child(GTK_BUTTON(btn), box);

  g_signal_connect(btn, "clicked", G_CALLBACK(btn_click_event), (gpointer)file->url);
  gtk_widget_add_css_class(btn, "game-button");

  return btn; 
}

