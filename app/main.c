#include "../include/folder.h"
#include "../include/gui.h"

#include <stdlib.h>
#include <gtk/gtk.h>

static display_component_t *b_d_component() {
    display_component_t *d_component = (display_component_t*) malloc(sizeof(display_component_t));
    if (d_component == NULL) {
        perror("Memory Allocation For d_component Failed");
        return NULL;
    }

    list_t *folder = load_folder();
    if (folder == NULL) {
        free(d_component);
        return NULL;
    }

    d_component->label = create_label(folder->size);

    d_component->games = (GtkWidget**) malloc(sizeof(GtkWidget*) * folder->size);
    if (d_component->games == NULL) {
        free(d_component);
        return NULL;
    }

    for (int i = 0; i < folder->size; i++) {
        list_item_t *item = (list_item_t*) folder->items[i];
        file_t *file = (file_t*)item->value; 
        d_component->games[i] = create_btn(file);
    }

    d_component->game_size = folder->size;

    return d_component;
}

static void activate(GtkApplication *app, gpointer user_data) {
    display_component_t *d_component = b_d_component();

    if (d_component == NULL) {
        return;
    }

    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

    GtkWidget *window;
    GdkDisplay *display;
    GtkCssProvider *provider;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();

    GFile *css_file = g_file_new_for_path(CSS_FILE_PATH);
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css_file);
    g_object_unref(provider);
    g_object_unref(css_file);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Game Library");
    gtk_window_set_default_size(GTK_WINDOW(window), DEFAULT_H, DEFAULT_W);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    gtk_widget_set_halign(d_component->label, GTK_ALIGN_START);
    gtk_widget_set_margin_top(d_component->label, 20);
    gtk_widget_set_margin_start(d_component->label, 20);
    gtk_box_append(GTK_BOX(main_box), d_component->label);

    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_box_append(GTK_BOX(main_box), scrolled_window);

    GtkWidget *flowbox = gtk_flow_box_new();
    gtk_widget_set_valign(flowbox, GTK_ALIGN_START);
    gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(flowbox), 30);
    gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flowbox), GTK_SELECTION_NONE);
    gtk_widget_set_margin_start(flowbox, 20);
    gtk_widget_set_margin_end(flowbox, 20);
    gtk_widget_set_margin_bottom(flowbox, 20);
    gtk_flow_box_set_column_spacing(GTK_FLOW_BOX(flowbox), 20);
    gtk_flow_box_set_row_spacing(GTK_FLOW_BOX(flowbox), 20);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), flowbox);

    for (int i = 0; i < d_component->game_size; i++) {
        gtk_flow_box_append(GTK_FLOW_BOX(flowbox), d_component->games[i]);
    }

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.css.example.fixed", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
