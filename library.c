#include <string.h>

#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>

/* default settings */
#define DEFAULT_SETTING1 NULL
#define DEFAULT_SETTING2 1
#define DEFAULT_SETTING3 FALSE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>

#include "library.h"



/* prototypes */
static void plugin_construct (XfcePanelPlugin *plugin);


/* register the plugin */
XFCE_PANEL_PLUGIN_REGISTER (plugin_construct);





//----------- Aktionen ----------------------------------------------------
//-------------------------------------------------------------------------

static void switch_sc_on(XfcePanelPlugin *plugin, GtkOrientation   orientation, Plugin    *sample){
    int status = system("/usr/bin/snapclient &");

    gtk_label_set_text(GTK_LABEL(sample->label1), "An");
}

static void switch_sc_off(XfcePanelPlugin *plugin, GtkOrientation   orientation, Plugin    *sample){

    char pids[1024];
    char * pid;
    char  killmsg[100];
    int i =0;
    int pidNr[64];

    FILE * fp = popen("pidof snapclient","r");
    FILE * f = fopen("/tmp/debug.txt", "w");

    fgets(pids, 1024, fp);

    pid = strtok(pids," ");

    fprintf(f, pid);

    while(pid != NULL){
        pidNr[i] = atoi(pid);
        //system(strcat("/usr/bin/kill ", pid));
        strcpy(killmsg, "/usr/bin/kill ");
        strcat(killmsg, pid);
        fprintf(f, killmsg);
        system(killmsg);
        pid=strtok(NULL, " ");
        ++i;
    }

    pclose(fp);
    fclose(f);

}

static void click_button(XfcePanelPlugin *plugin, GtkOrientation   orientation, Plugin    *sample){

    gtk_label_set_text(GTK_LABEL(sample->label1), "Sample");

    //Menu Öffnen
    gtk_menu_popup_at_pointer(GTK_MENU(sample->menu), NULL);


}



//-----------------Create New Plugin with GTKwidgets and co ------------------
//-----------------------------------------------------------------------------
static Plugin * new_plugin (XfcePanelPlugin *plugin){
    Plugin   *pluginData;
    GtkOrientation  orientation;
    //GtkWidget      *label1;

    /* allocate memory for the plugin structure */
    pluginData = g_slice_new0 (Plugin);

    /* pointer to plugin */
    pluginData->plugin = plugin;

    /* read the user settings */
    //sample_read (sample);

    /* get the current orientation */
    orientation = xfce_panel_plugin_get_orientation (plugin);

    /* create some panel widgets */
    pluginData->ebox = gtk_event_box_new ();
    gtk_widget_show (pluginData->ebox);

    pluginData->hvbox = gtk_box_new (orientation, 2);
    gtk_widget_show (pluginData->hvbox);
    gtk_container_add (GTK_CONTAINER (pluginData->ebox), pluginData->hvbox);

    /* some sample widgets */
    pluginData->label1 = gtk_label_new (_("SaMple"));
    gtk_widget_show (pluginData->label1);
    gtk_box_pack_start (GTK_BOX (pluginData->hvbox), pluginData->label1, FALSE, FALSE, 0);

    pluginData-> button = xfce_panel_create_button();
    gtk_widget_show(pluginData->button);
    gtk_button_set_label(GTK_BUTTON(pluginData->button), "Start");
    gtk_box_pack_start(GTK_BOX(pluginData->hvbox), pluginData->button, FALSE, FALSE, 0);



    //Submenu

    // Vesuch Menu zu erstellen
    pluginData->menu = gtk_menu_new();

    pluginData->snapclientAn = gtk_menu_item_new_with_label("Snapclient An");
    pluginData->snapclientAus = gtk_menu_item_new_with_label("Snapclient Aus");

    gtk_menu_shell_append(GTK_MENU_SHELL(pluginData->menu), pluginData->snapclientAn);
    gtk_menu_shell_append(GTK_MENU_SHELL(pluginData->menu), pluginData->snapclientAus);

    g_signal_connect (pluginData->snapclientAn, "activate", G_CALLBACK (switch_sc_on), pluginData);
    g_signal_connect (pluginData->snapclientAus, "activate", G_CALLBACK (switch_sc_off), pluginData);

    gtk_widget_show_all (GTK_WIDGET (pluginData->menu));

    return pluginData;
}



//------------- Plugin Constructor -------------------------------------
//----------------------------------------------------------------------

static void plugin_construct (XfcePanelPlugin *plugin){

    Plugin * pluginData;

    /* setup transation domain */
    //xfce_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");
    //xfce_textdomain("libtestpluginxfce", "/usr/lib/xfce4/panel-plugins/", "UTF-8");

    /* create the plugin */
    pluginData = new_plugin(plugin);

    /* add the ebox to the panel */
    gtk_container_add (GTK_CONTAINER (plugin), pluginData->ebox);

    /* show the panel's right-click menu on this ebox */
    xfce_panel_plugin_add_action_widget (plugin, pluginData->ebox);

//    /* connect plugin signals */
//    g_signal_connect (G_OBJECT (plugin), "free-data",
//                      G_CALLBACK (sample_free), sample);
//
//    g_signal_connect (G_OBJECT (plugin), "save",
//                      G_CALLBACK (sample_save), sample);
//
//    g_signal_connect (G_OBJECT (plugin), "size-changed",
//                      G_CALLBACK (sample_size_changed), sample);
//
//    g_signal_connect (G_OBJECT (plugin), "orientation-changed",
//                      G_CALLBACK (sample_orientation_changed), sample);
////
//    /* show the configure menu item and connect signal */
//    xfce_panel_plugin_menu_show_configure (plugin);
//    g_signal_connect (G_OBJECT (plugin), "configure-plugin", G_CALLBACK (sample_configure), sample);
////
//    /* show the about menu item and connect signal */
//    xfce_panel_plugin_menu_show_about (plugin);
//    g_signal_connect (G_OBJECT (plugin), "about", G_CALLBACK (sample_about), NULL);
//

    g_signal_connect (pluginData->button, "clicked", G_CALLBACK (click_button), pluginData);

}
