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

static void switch_sc_on(XfcePanelPlugin *plugin, Plugin    *pluginData){
    int status = system("/usr/bin/snapclient &");

    GtkWidget* button_image = gtk_image_new_from_pixbuf(pluginData->buf_icon_on);
    gtk_button_set_image(GTK_BUTTON(pluginData->button), button_image);
}

static void switch_sc_off(XfcePanelPlugin *plugin, Plugin    *pluginData){

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
    fprintf(f, "%i\n", pluginData);

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


    GtkWidget* button_image = gtk_image_new_from_pixbuf(pluginData->buf_icon_off);
    gtk_button_set_image(GTK_BUTTON(pluginData->button), button_image);

}

static void click_button(XfcePanelPlugin *plugin, GtkOrientation   orientation, Plugin    *sample){


    //Menu Öffnen
    gtk_menu_popup_at_pointer(GTK_MENU(sample->menu), NULL);


}



//-----------------Create New Plugin with GTKwidgets and co ------------------
//-----------------------------------------------------------------------------
void new_plugin (XfcePanelPlugin *plugin, Plugin * pluginData){
    //Plugin   *pluginData;
    GtkOrientation  orientation;
    //GtkWidget      *label1;

    /* allocate memory for the plugin structure */


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

    pluginData-> button = xfce_panel_create_button();
    gtk_widget_show(pluginData->button);
    //gtk_button_set_label(GTK_BUTTON(pluginData->button), "SC");


    // Look for icon ----------------------------------------------------------------------------------------
    // Install icon: xdg-icon-resource install --size 16 snapclientPlugin-sc_16.png snapclientPlugin-sc^C
    //------------------------------------------------------------------------------------------------------
    GtkIconTheme *	theme = gtk_icon_theme_get_default ();
    pluginData->buf_icon_on = gtk_icon_theme_load_icon_for_scale (theme, "snapclientPlugin-sc", 20, 1, GTK_ICON_LOOKUP_FORCE_SIZE,NULL);
    pluginData->buf_icon_off = gtk_icon_theme_load_icon_for_scale (theme, "snapclientPlugin-sc-off", 20, 1, GTK_ICON_LOOKUP_FORCE_SIZE,NULL);
    GtkWidget* button_image = gtk_image_new_from_pixbuf(pluginData->buf_icon_off);


    //Set icon on button
    gtk_button_set_image(GTK_BUTTON(pluginData->button), button_image);
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

    //return pluginData;
}





//------------ About----------------------------------------------------
//----------------------------------------------------------------------
void about (XfcePanelPlugin *plugin)
{

    GdkPixbuf *icon;

    const gchar *auth[] =
            {
                    "Lukas Merkle",
                    NULL
            };

    icon = xfce_panel_pixbuf_from_source ("/home/lukas/CLionProjects/snapcastXfcePlugin/icon/sc_kl.ico", NULL, 32);
    gtk_show_about_dialog (NULL,
                           "logo",         icon,
                           "license",      xfce_get_license_text (XFCE_LICENSE_TEXT_GPL),
                           "version",      "v01",
                           "program-name", "Snapclient Xfce Plugin",
                           "comments",     _("Toggle Snapclient from Xfce-Panel"),
                           "website",      "https://github.com/luckyluke751/snapcastXfcePlugin",
                           "copyright",    _("Copyright \xc2\xa9 2017 Lukas Merkle\n"),
                           "authors",      auth,
                           NULL);

    if (icon)
        g_object_unref (G_OBJECT (icon));
}



//------------- Plugin Constructor -------------------------------------
//----------------------------------------------------------------------

void plugin_construct (XfcePanelPlugin *plugin){

    Plugin * pluginData;
    pluginData = g_slice_new0 (Plugin);
    /* setup transation domain */
    //xfce_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");
    //xfce_textdomain("libtestpluginxfce", "/usr/lib/xfce4/panel-plugins/", "UTF-8");

    /* create the plugin */
    new_plugin(plugin, pluginData);

    /* add the ebox to the panel */
    gtk_container_add (GTK_CONTAINER (plugin), pluginData->ebox);

    /* show the panel's right-click menu on this ebox */
    xfce_panel_plugin_add_action_widget (plugin, pluginData->ebox);

//    /* connect plugin signals */
//    g_signal_connect (G_OBJECT (plugin), "free-data",
//                      G_CALLBACK (sample_free), pluginData);
//
//    g_signal_connect (G_OBJECT (plugin), "save",
//                      G_CALLBACK (sample_save), pluginData);
//
//    g_signal_connect (G_OBJECT (plugin), "size-changed",
//                      G_CALLBACK (sample_size_changed), pluginData);
//
//    g_signal_connect (G_OBJECT (plugin), "orientation-changed",
//                      G_CALLBACK (sample_orientation_changed), pluginData);

   /* show the configure menu item and connect signal */
//   xfce_panel_plugin_menu_show_configure (plugin);
//   g_signal_connect (G_OBJECT (plugin), "configure-plugin", G_CALLBACK (sample_configure), pluginData);

    /* show the about menu item and connect signal */
    xfce_panel_plugin_menu_show_about (plugin);
    g_signal_connect (G_OBJECT (plugin), "about", G_CALLBACK (about), NULL);

    g_signal_connect (pluginData->button, "clicked", G_CALLBACK (click_button), pluginData);

}
