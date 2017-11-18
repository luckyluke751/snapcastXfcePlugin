#ifndef SNAPCASTXFCEPLUGIN_LIBRARY_H
#define SNAPCASTXFCEPLUGIN_LIBRARY_H


/* plugin structure */
typedef struct{
    XfcePanelPlugin *plugin;

    /* panel widgets */
    GtkWidget       *ebox;
    GtkWidget       *hvbox;
    GtkWidget       *button;


    GtkWidget * menu;

    GtkWidget * snapclientAn;
    GtkWidget * snapclientAus;

    /* sample settings */
    gchar           *setting1;
    gint             setting2;
    gboolean         setting3;
}
Plugin;

#endif