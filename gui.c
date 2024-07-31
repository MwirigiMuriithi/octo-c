#include <gtk/gtk.h>
#include "function.h"

// Global variables
static struct Farm farm;
static GtkWidget *animal_count_entry;

// Function declarations
void on_add_button_clicked(GtkButton *button, gpointer user_data);
void on_view_button_clicked(GtkButton *button, gpointer user_data);
void on_exit_button_clicked(GtkButton *button, gpointer user_data);

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *add_button;
    GtkWidget *view_button;
    GtkWidget *exit_button;
    GtkWidget *label;
    
    gtk_init(&argc, &argv);

    // Initialize the farm
    initializeFarm(&farm, MAX_ANIMALS);

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Farm Management");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create and add widgets
    label = gtk_label_new("Farm Management System");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    animal_count_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), animal_count_entry, FALSE, FALSE, 0);

    add_button = gtk_button_new_with_label("Add Animals");
    gtk_box_pack_start(GTK_BOX(box), add_button, FALSE, FALSE, 0);
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_button_clicked), NULL);

    view_button = gtk_button_new_with_label("View Farm Inventory");
    gtk_box_pack_start(GTK_BOX(box), view_button, FALSE, FALSE, 0);
    g_signal_connect(view_button, "clicked", G_CALLBACK(on_view_button_clicked), NULL);

    exit_button = gtk_button_new_with_label("Exit");
    gtk_box_pack_start(GTK_BOX(box), exit_button, FALSE, FALSE, 0);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_button_clicked), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    // Cleanup
    cleanupFarm(&farm);

    return 0;
}

void on_add_button_clicked(GtkButton *button, gpointer user_data) {
    const char *text = gtk_entry_get_text(GTK_ENTRY(animal_count_entry));
    int numAnimals = atoi(text);
    if (numAnimals > 0) {
        populateFarm(&farm, numAnimals);
    }
}

void on_view_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    char output[4096];

    // Create a dialog to display the farm inventory
    dialog = gtk_dialog_new_with_buttons("Farm Inventory",
                                         NULL,
                                         GTK_DIALOG_MODAL,
                                         "Close",
                                         GTK_RESPONSE_CLOSE,
                                         NULL);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_CLOSE);
    buffer = gtk_text_buffer_new(NULL);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(gtk_bin_get_child(GTK_BIN(dialog))), buffer);

    // Prepare farm inventory details
    snprintf(output, sizeof(output),
             "+------------+------------------+-----+----------+--------+--------+----------+------------------+\n"
             "| Tag Number | Name             | Age | Type     | Weight | Gender | Offsprings | Breed            |\n"
             "+------------+------------------+-----+----------+--------+--------+----------+------------------+\n");

    for (int i = 0; i < farm.animalCount; ++i) {
        const char *typeStr = animalTypeToString(farm.animals[i].type);
        const char *genderStr = genderToString(farm.animals[i].gender);
        const char *breedStr = breedToString(&farm.animals[i]);

        snprintf(output + strlen(output), sizeof(output) - strlen(output),
                 "| %10d | %-16s | %3d | %-8s | %6.2f | %-6s | %10d | %-16s |\n",
                 farm.animals[i].tagNumber,
                 farm.animals[i].name,
                 farm.animals[i].age,
                 typeStr,
                 farm.animals[i].weight,
                 genderStr,
                 farm.animals[i].numOffsprings,
                 breedStr);
    }

    snprintf(output + strlen(output),
             sizeof(output) - strlen(output),
             "+------------+------------------+-----+----------+--------+--------+----------+------------------+\n");

    gtk_text_buffer_set_text(buffer, output, -1);
    gtk_widget_show_all(dialog);
}

void on_exit_button_clicked(GtkButton *button, gpointer user_data) {
    gtk_main_quit();
}
