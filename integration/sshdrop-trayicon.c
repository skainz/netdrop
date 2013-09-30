#include<gtk/gtk.h>
#include<libnotify/notify.h>
#include<stdio.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <string.h>

NotifyNotification *example;
//int counter=0;
//char info[256];
char preview[256];

FILE* fp;
char* filename;
long file_size;
char* data;
char* sender;

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

  FILE *pf;
  char command[256];

 
  GtkClipboard* cb=gtk_clipboard_get(GDK_SELECTION_PRIMARY);

  gtk_clipboard_set_text(cb,data,-1);

  //  printf ("%s\n",user_data);
 
 

}


void status_icon_notification_closed_cb (NotifyNotification *notification,
				    gpointer  icon)
{

  printf ("Close reason:%d\n",notify_notification_get_closed_reason(notification));

  printf("popup closed\n");
}

void
callback (GFileMonitor *mon, GFile *first, GFile *second, GFileMonitorEvent event, gpointer udata)
{
  if (event == G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT)
    {

 notify_notification_close(example,NULL);
  //char *msg = decode (event);
  printf ("file changed %s\n",g_file_get_path(first));
  //  printf ("event: %d\n",event);
  sender=g_file_get_basename(first);
  fp=fopen(g_file_get_path(first),"r");
  if (!(fp==(FILE*)NULL))
    {
      fseek(fp,0,SEEK_END);
      file_size=ftell(fp);
      printf("filesize: %d\n",file_size);
      fseek(fp,0,SEEK_SET);
      data=malloc(sizeof(char)*file_size+1);
      data[file_size]=0;
      fread(data,1,file_size,fp);
      fclose(fp);
      
      if (data[file_size-1]=='\n')
	{
	  data[file_size-1]=0;
	}
      
      //   sprintf(info," %d",counter);
      GError *error = NULL;
      memcpy(preview,data,256);
      preview[255]=0;
      notify_notification_update(example,sender,preview,NULL);
      notify_notification_show(example,&error);
    } else
    {
      printf("Unable to open file %s\n",filename);
    }
 
    }

}




int main(int argc, char **argv)
{

  GFile *file;
  GFileMonitor *mon;


  if (argc<2)
    {
      printf ("Usage: %s <directory>\n",argv[0]);
      exit(1);
    }

  g_type_init();
  file=g_file_new_for_path(argv[1]);
  mon=g_file_monitor(file,G_FILE_MONITOR_EVENT_CHANGED,NULL,NULL);

  g_signal_connect (mon, "changed", G_CALLBACK (callback), NULL);
  

    // initialize gtk
    gtk_init(&argc,&argv);
    
    char name[40] = "SSHDrop";
    
    // initiate notify
    notify_init(name);
    
    // create a new notification
   
    example = notify_notification_new(name,"",NULL);
    
      
    addIcon(example);
    notify_notification_add_action(example,"copy","Copy to Clipboard",(NotifyActionCallback)notif_libnotify_callback_open,name,NULL);

    g_signal_connect(example,"closed",G_CALLBACK (status_icon_notification_closed_cb),NULL);


    // set the timeout of the notification to the default value
    notify_notification_set_timeout(example,NOTIFY_EXPIRES_DEFAULT);
    
    // set the category so as to tell what kind it is
    char category[30] = "Testing Notifications";
    notify_notification_set_category(example,category);
    
    // set the urgency level of the notification
    notify_notification_set_urgency (example,NOTIFY_URGENCY_NORMAL);
    


    gtk_main();
}


 // gtk_main_quit();
//http://www.youtube.com/watch?v=yX8yrOAjfKM
