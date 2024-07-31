#include <gtk/gtk.h>
#include "gui.h"
#include "db.h"

// Global variable for the treeview
static GtkWidget *animal_treeview;

// Function to create the treeview and add it to the given box
static void create_treeview(GtkWidget *box) {
    GtkListStore *store;
    GtkTreeIter iter;

    // Create a new GtkListStore with columns for animal data
    store = gtk_list_store_new(8, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_DOUBLE, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING);

    // Create the treeview and set the model
    animal_treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    // Create columns
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Tag Number", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(animal_treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(animal_treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Age", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(animal_treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(animal_treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Weight", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(animal_treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Gender", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(animal_treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Offsprings", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(animal_treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Breed", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(animal_treeview), column);

    gtk_box_pack_start(GTK_BOX(box), animal_treeview, TRUE, TRUE, 0);
    gtk_widget_show_all(animal_treeview);
}

// Callback function to handle the "Load Animals" button click
static void on_load_animals_clicked(GtkButton *button, gpointer user_data) {
    struct Farm *farm = (struct Farm *)user_data;
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(animal_treeview)));
    GtkTreeIter iter;

    gtk_list_store_clear(store);

    loadAnimalsFromDatabase(farm);

    for (int i = 0; i < farm->animalCount; ++i) {
        const gchar *type_str = animalTypeToString(farm->animals[i].type);
        const gchar *gender_str = genderToString(farm->animals[i].gender);
        const gchar *breed_str = breedToString(&farm->animals[i]);

        // Check for NULL values
        if (!type_str || !gender_str || !breed_str) {
            g_warning("Received NULL string from animalTypeToString, genderToString, or breedToString.");
            continue;
        }

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, farm->animals[i].tagNumber,
            1, farm->animals[i].name,
            2, farm->animals[i].age,
            3, type_str,
            4, farm->animals[i].weight,
            5, gender_str,
            6, farm->animals[i].numOffsprings,
            7, breed_str,
            -1
        );
    }
}


// Callback function to handle the "Add Animal" button click
static void on_add_animal_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *name_entry, *age_entry, *weight_entry;
    GtkWidget *type_combo, *gender_combo, *offspring_entry, *breed_entry;
    struct Farm *farm = (struct Farm *)user_data;

    dialog = gtk_dialog_new_with_buttons("Add Animal",
                                         GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                         GTK_DIALOG_MODAL,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_ACCEPT,
                                         GTK_STOCK_CANCEL,
                                         GTK_RESPONSE_REJECT,
                                         NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    // Create and add widgets for animal details
    name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Name:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);

    age_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Age:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), age_entry, 1, 1, 1, 1);

    weight_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Weight:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), weight_entry, 1, 2, 1, 1);

    type_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "Cow");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "Horse");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "Sheep");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "Pig");
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Type:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), type_combo, 1, 3, 1, 1);

    gender_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gender_combo), "Male");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gender_combo), "Female");
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Gender:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gender_combo, 1, 4, 1, 1);

    offspring_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Offsprings:"), 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), offspring_entry, 1, 5, 1, 1);

    breed_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Breed:"), 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), breed_entry, 1, 6, 1, 1);

    gtk_widget_show_all(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        struct Animal new_animal;
        // Retrieve the input values and add the animal to the farm
        strcpy(new_animal.name, gtk_entry_get_text(GTK_ENTRY(name_entry)));
        new_animal.age = atoi(gtk_entry_get_text(GTK_ENTRY(age_entry)));
        new_animal.weight = atof(gtk_entry_get_text(GTK_ENTRY(weight_entry)));
        new_animal.type = gtk_combo_box_get_active(GTK_COMBO_BOX(type_combo));
        new_animal.gender = gtk_combo_box_get_active(GTK_COMBO_BOX(gender_combo));
        new_animal.numOffsprings = atoi(gtk_entry_get_text(GTK_ENTRY(offspring_entry)));
        new_animal.breed.cowBreed = atoi(gtk_entry_get_text(GTK_ENTRY(breed_entry))); // Adjust based on type

        addAnimalToDatabase(&new_animal);
        on_load_animals_clicked(NULL, user_data); // Refresh the list after adding
    }

    gtk_widget_destroy(dialog);
}

// Function to set up the GUI
void setup_gui(GtkApplication *app, struct Farm *farm) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *load_button;
    GtkWidget *add_button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Farm Management");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    create_treeview(box);

    load_button = gtk_button_new_with_label("Load Animals");
    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load_animals_clicked), farm);
    gtk_box_pack_start(GTK_BOX(box), load_button, FALSE, FALSE, 0);

    add_button = gtk_button_new_with_label("Add Animal");
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_animal_clicked), farm);
    gtk_box_pack_start(GTK_BOX(box), add_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}

