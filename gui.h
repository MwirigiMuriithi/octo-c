#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "function.h"

void activate(GtkApplication *app, gpointer user_data);
void setup_gui(GtkApplication *app, struct Farm *farm);

#endif
