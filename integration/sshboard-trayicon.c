
#include<libnotify/notify.h>

void addIcon( NotifyNotification * notify )
{
    int size = 32;
    GtkIconTheme * theme;
    GdkPixbuf * icon;

    gtk_icon_size_lookup( GTK_ICON_SIZE_DIALOG, &size, &size );
    theme = gtk_icon_theme_get_default( );
         icon = gtk_icon_theme_load_icon( theme, "edit-paste", size, 0, NULL );
 
   if( icon != NULL )
    {
        notify_notification_set_icon_from_pixbuf( notify, icon );
        g_object_unref( icon );
    }
}

static void notif_libnotify_callback_open ( NotifyNotification *n, gchar *action, gpointer user_data ) {
  //	g_assert(action != NULL);
  //	g_assert(strcmp(action, "open") == 0);
  GtkClipboard* cb=gtk_clipboard_get(GDK_SELECTION_PRIMARY);

  gtk_clipboard_set_text(cb,"foobar",-1);
  //  gtk.Clipboard();
  printf ("%s\n",user_data);
}



int main(int argc, char **argv)
{
    // initialize gtk
    gtk_init(&argc,&argv);
    
    char name[40] = "SSHBoard";
    
    // initiate notify
    notify_init(name);
    
    // create a new notification
    NotifyNotification *example;
    example = notify_notification_new(name,"Checking it out\nghjghjgjg\nssdfsdfsd",NULL,NULL);
    
    /*  Status Icon is not working properly */
    // create an icon for the notification

    //    GtkStatusIcon *icon = gtk_status_icon_new_from_stock (GTK_STOCK_YES);
    //gtk_status_icon_set_visible(icon,TRUE);
    // attach that icon to the notification
    //    notify_notification_attach_to_status_icon (example,icon);
    
    addIcon(example);
    notify_notification_add_action(example,"copy","Copy to Clipboard",(NotifyActionCallback)notif_libnotify_callback_open,name,NULL);

    //    notify_notification_add_action(example,"paste","Open Feed",(NotifyActionCallback)notif_libnotify_callback_open,NULL,NULL);
    // notify_notification_add_action(example,"open","Open Feed",(NotifyActionCallback)notif_libnotify_callback_open,NULL,NULL);


    // set the timeout of the notification to 3 secs
    notify_notification_set_timeout(example,3000);
    
    // set the category so as to tell what kind it is
    char category[30] = "Testing Notifications";
    notify_notification_set_category(example,category);
    
    // set the urgency level of the notification
    notify_notification_set_urgency (example,NOTIFY_URGENCY_CRITICAL);
    
    GError *error = NULL;
    notify_notification_show(example,&error);

    gtk_main();
}
