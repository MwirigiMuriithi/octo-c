#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "db.h" // Assuming this includes necessary definitions for the Farm structure and animal data handling

// Function to initialize the GUI
void setup_gui(GtkApplication *app, struct Farm *farm);

#endif // GUI_H
