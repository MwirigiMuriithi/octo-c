#include <gtk/gtk.h>
#include "gui.h"
#include "function.h"
#include "db.h"

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;
    struct Farm farm;

    initDatabase();
    initializeFarm(&farm, MAX_ANIMALS);

    app = gtk_application_new("com.example.farm", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), &farm);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    cleanupFarm(&farm);
    g_object_unref(app);

    return status;
}
