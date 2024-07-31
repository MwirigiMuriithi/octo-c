#include <gtk/gtk.h>
#include "gui.h"
#include "db.h"

static void on_load_animals_clicked(GtkButton *button, gpointer user_data) {
    struct Farm *farm = (struct Farm *)user_data;
    loadAnimalsFromDatabase(farm);
    printFarm(farm);
}

static void on_add_animal_clicked(GtkButton *button, gpointer user_data) {
    struct Farm *farm = (struct Farm *)user_data;
    int numAnimals = validateIntegerInput("Enter the number of animals to add: ");
    populateFarm(farm, numAnimals);
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *load_button;
    GtkWidget *add_button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Farm Management");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    load_button = gtk_button_new_with_label("Load Animals");
    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load_animals_clicked), user_data);
    gtk_box_pack_start(GTK_BOX(box), load_button, TRUE, TRUE, 0);

    add_button = gtk_button_new_with_label("Add Animals");
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_animal_clicked), user_data);
    gtk_box_pack_start(GTK_BOX(box), add_button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}

void setup_gui(GtkApplication *app, struct Farm *farm) {
    activate(app, farm);
}
